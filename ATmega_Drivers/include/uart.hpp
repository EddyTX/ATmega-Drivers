#ifndef ATMEGADRIVERS_UART_HPP_INCLUDED
#define ATMEGADRIVERS_UART_HPP_INCLUDED

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "project_defines.hpp"

class UartDriver
{
	public:
	
	static UartDriver& GetInstance()
	{
		static UartDriver instance;
		return instance;
	}

	
	void Init(uint32_t baud);
	void SendChar(char c);
	void SendString(const char* str);
	bool ReadChar(char &c);

	
	void ReceiveCharISR(uint8_t c); 
	void SendCharISR();              

	private:
	
	UartDriver() {}
	~UartDriver() {}
	UartDriver(const UartDriver&) = delete;
	UartDriver& operator=(const UartDriver&) = delete;

	
	uint8_t rxBuffer[UART_RX_BUFFER_SIZE];
	volatile uint8_t head = 0;
	volatile uint8_t tail = 0;
	
	
	uint8_t txBuffer[UART_TX_BUFFER_SIZE];
	volatile uint8_t txHead = 0;
	volatile uint8_t txTail = 0;
};

#endif 