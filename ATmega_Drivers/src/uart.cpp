#include "../include/uart.hpp"
#include "../include/project_defines.hpp"
#include <avr/interrupt.h>


void UartDriver::Init(uint32_t baud)
{
	uint16_t ubrr = F_CPU/16/baud - 1;
	UBRR0H = (uint8_t)(ubrr >> 8);
	UBRR0L = (uint8_t)(ubrr & 0xFF);
	
	UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}


void UartDriver::SendChar(char c)
{
	
	uint8_t nextHead = (txHead + 1) & (UART_TX_BUFFER_SIZE - 1);
	
	while (nextHead == txTail)
	{
			
	}

	txBuffer[txHead] = c;
	txHead = nextHead;
	
	UCSR0B |= (1 << UDRIE0);
}

void UartDriver::SendString(const char* str)
{
	
	while(*str)
	{
		SendChar(*str++);
	}
}


bool UartDriver::ReadChar(char &c)
{
	
	uint8_t sreg_backup = SREG;
	cli();

	if(head == tail)
	{
		SREG = sreg_backup;
		return false;
	}

	c = rxBuffer[tail];
	tail = (tail + 1) & (UART_RX_BUFFER_SIZE - 1); 

	SREG = sreg_backup;
	return true;
}


void UartDriver::ReceiveCharISR(uint8_t c)
{
	uint8_t next = (head + 1) & (UART_RX_BUFFER_SIZE - 1); 
	if(next != tail)
	{
		rxBuffer[head] = c;
		head = next;
	}
	
}


void UartDriver::SendCharISR()
{
	
	if (txHead == txTail)
	{
		
		UCSR0B &= ~(1 << UDRIE0);
	}
	else
	{
		
		char c = txBuffer[txTail];
		txTail = (txTail + 1) & (UART_TX_BUFFER_SIZE - 1); 
		
		UDR0 = c;
	}
}



ISR(USART_RX_vect)
{
	UartDriver::GetInstance().ReceiveCharISR(UDR0);
}


ISR(USART_UDRE_vect)
{
	UartDriver::GetInstance().SendCharISR();
}