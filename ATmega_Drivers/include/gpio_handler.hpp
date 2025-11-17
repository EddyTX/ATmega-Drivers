#pragma once

#include "iendpoint_handler.hpp"
#include "icomm_channel.hpp"
#include "gpio.hpp" // Pentru GpioDriver Singleton

class GpioHandler : public IEndpointHandler
{
	public:
	GpioHandler(ICommChannel& comm);
	bool Handle(const char* method, const char* path) override;

	private:
	void SendResponse(const char* code, const char* contentType, const char* body);
	void SendOK();

	ICommChannel& comm_;
	GpioDriver& gpio_; // O referin?? la Singleton
};