#pragma once

#include "iendpoint_handler.hpp"
#include "icomm_channel.hpp"
#include "timer.hpp" 

class TimerHandler : public IEndpointHandler
{
	public:
	
	TimerHandler(ICommChannel& comm, TimerDriver& timerDriver);

	bool Handle(const char* method, const char* path) override;

	private:
	
	void SendResponse(const char* code, const char* contentType, const char* body);

	ICommChannel& comm_;
	TimerDriver& timerDriver_;
};