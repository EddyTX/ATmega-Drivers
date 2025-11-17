#pragma once

#include "icomm_channel.hpp"
#include "uart.hpp"

class SerialChannel : public ICommChannel
{
	public:
	void Init(uint32_t baud) override;
	void SendData(const uint8_t* data, size_t size) override;
	bool ReceiveData(uint8_t& data) override;

	void TestEcho(); // <-- ADAUG? ACEAST? LINIE

	private:
	// UartDriver uart_driver_; // Aceasta este goal? de când am trecut la Singleton
};