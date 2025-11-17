#include "../include/serial_channel.hpp"


void SerialChannel::Init(uint32_t baud)
{
	UartDriver::GetInstance().Init(baud);
}

void SerialChannel::SendData(const uint8_t* data, size_t size)
{
	for(size_t i = 0; i < size; ++i)
	{
		UartDriver::GetInstance().SendChar(data[i]);
	}
}

bool SerialChannel::ReceiveData(uint8_t& data)
{
	char c;
	if(UartDriver::GetInstance().ReadChar(c))
	{
		data = static_cast<uint8_t>(c);
		return true;
	}
	return false;
}


void SerialChannel::TestEcho()
{
	uint8_t data;
	
	if(this->ReceiveData(data))
	{
		this->SendData(&data, 1);
	}
}