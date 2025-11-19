#pragma once

#include "iendpoint_handler.hpp"
#include "icomm_channel.hpp"
#include "pwm.hpp"

class PwmHandler : public IEndpointHandler
{
	public:
	PwmHandler(ICommChannel& comm);
	bool Handle(const char* method, const char* path) override;

	private:
	void SendOK();
	ICommChannel& comm_;
	PwmDriver& pwm_;
};