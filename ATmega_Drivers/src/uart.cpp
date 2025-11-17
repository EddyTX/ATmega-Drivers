#include "../include/uart.hpp"
#include "../include/project_defines.hpp"
#include <avr/interrupt.h>

// --- Ini?ializarea ---
void UartDriver::Init(uint32_t baud)
{
	uint16_t ubrr = F_CPU/16/baud - 1;
	UBRR0H = (uint8_t)(ubrr >> 8);
	UBRR0L = (uint8_t)(ubrr & 0xFF);

	// Activ?m RX, TX ?i Întreruperea de RX (RXCIE0).
	// NU activ?m întreruperea de TX (UDRIE0) înc?. Ea va fi activat?
	// doar când SendChar() are ceva de trimis.
	UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

// --- Transmisie (TX) - MODIFICAT (Non-Blocking) ---
void UartDriver::SendChar(char c)
{
	// Calculeaz? urm?torul index pentru head-ul buffer-ului TX
	// Folosim '&' în loc de '%' pentru vitez? (buffer size = 128)
	uint8_t nextHead = (txHead + 1) & (UART_TX_BUFFER_SIZE - 1);

	// A?teapt? (spin-wait) DOAR dac? buffer-ul TX este plin
	while (nextHead == txTail)
	{
		// Buffer-ul e plin, a?teapt? ca ISR-ul s? trimit? un caracter
		// ?i s? elibereze un loc.
	}

	txBuffer[txHead] = c;
	txHead = nextHead;

	// Activeaz? întreruperea "Data Register Empty" (UDRIE0)
	// Asta va declan?a ISR(USART_UDRE_vect) imediat ce hardware-ul e gata
	UCSR0B |= (1 << UDRIE0);
}

void UartDriver::SendString(const char* str)
{
	// Aceast? func?ie e acum non-blocant?
	// Doar umple buffer-ul TX, ISR-ul se ocup? de trimitere
	while(*str)
	{
		SendChar(*str++);
	}
}

// --- Recep?ie (RX) - (Neschimbat, dar cu optimizarea '&') ---
bool UartDriver::ReadChar(char &c)
{
	// Sec?iune critic? pentru a citi 'head' ?i 'tail' în siguran??
	uint8_t sreg_backup = SREG;
	cli();

	if(head == tail)
	{
		// Buffer gol
		SREG = sreg_backup;
		return false;
	}

	c = rxBuffer[tail];
	tail = (tail + 1) & (UART_RX_BUFFER_SIZE - 1); // Optimizare '&'

	SREG = sreg_backup;
	return true;
}

// ==========================================================
// --- Implement?rile metodelor pentru ISR-uri ---
// ==========================================================

// --- Metoda pentru ISR-ul de RX ---
void UartDriver::ReceiveCharISR(uint8_t c)
{
	uint8_t next = (head + 1) & (UART_RX_BUFFER_SIZE - 1); // Optimizare '&'
	if(next != tail)
	{
		rxBuffer[head] = c;
		head = next;
	}
	// else: Buffer plin, caracterul este pierdut (comportament normal)
}

// --- Metoda (NOU?) pentru ISR-ul de TX ---
void UartDriver::SendCharISR()
{
	// Verific? dac? mai este ceva de trimis
	if (txHead == txTail)
	{
		// Buffer-ul TX este gol.
		// Dezactiveaz? întreruperea UDRE pentru a nu se mai declan?a inutil.
		UCSR0B &= ~(1 << UDRIE0);
	}
	else
	{
		// Mai sunt date. Ia un caracter din buffer-ul TX...
		char c = txBuffer[txTail];
		txTail = (txTail + 1) & (UART_TX_BUFFER_SIZE - 1); // Optimizare '&'
		
		// ...?i pune-l în registrul hardware (UDR0) pentru a fi trimis
		UDR0 = c;
	}
}

// ==========================================================
// --- Vectorii de Întrerupere (ISR) ---
// ==========================================================

// ISR-ul de RX (existent)
// Se declan?eaz? când un caracter A SOSIT
ISR(USART_RX_vect)
{
	UartDriver::GetInstance().ReceiveCharISR(UDR0);
}

// ISR-ul de TX (NOU)
// Se declan?eaz? când hardware-ul e GATA S? TRIMIT? (registrul e gol)
ISR(USART_UDRE_vect)
{
	UartDriver::GetInstance().SendCharISR();
}