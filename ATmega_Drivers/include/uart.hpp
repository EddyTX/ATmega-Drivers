#ifndef ATMEGADRIVERS_UART_HPP_INCLUDED
#define ATMEGADRIVERS_UART_HPP_INCLUDED

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "project_defines.hpp"

class UartDriver
{
	public:
	// --- SINGLETON ACCESSOR ---
	static UartDriver& GetInstance()
	{
		static UartDriver instance;
		return instance;
	}

	// --- METODE PUBLICE ---
	void Init(uint32_t baud);
	void SendChar(char c);
	void SendString(const char* str);
	bool ReadChar(char &c);

	// --- METODE PENTRU ISR-URI ---
	void ReceiveCharISR(uint8_t c); // Pentru RX
	void SendCharISR();              // Pentru TX (NOU)

	private:
	// --- SINGLETON CONSTRUCTORS (PRIVATE) ---
	UartDriver() {}
	~UartDriver() {}
	UartDriver(const UartDriver&) = delete;
	UartDriver& operator=(const UartDriver&) = delete;

	// --- RX BUFFER ---
	uint8_t rxBuffer[UART_RX_BUFFER_SIZE];
	volatile uint8_t head = 0;
	volatile uint8_t tail = 0;
	
	// --- TX BUFFER (NOU) ---
	uint8_t txBuffer[UART_TX_BUFFER_SIZE];
	volatile uint8_t txHead = 0;
	volatile uint8_t txTail = 0;
};

#endif /* ATMEGADRIVERS_UART_HPP_INCLUDED */