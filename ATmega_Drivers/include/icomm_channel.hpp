#pragma once

#include <stdint.h>
#include <stddef.h>

class ICommChannel
{
	public:
	virtual void Init(uint32_t baud) = 0;
	virtual void SendData(const uint8_t* data, size_t size) = 0;
	virtual bool ReceiveData(uint8_t& data) = 0;

	virtual ~ICommChannel() = default;
};
