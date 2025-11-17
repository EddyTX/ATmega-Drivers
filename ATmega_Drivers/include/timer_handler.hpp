#pragma once

#include "iendpoint_handler.hpp"
#include "icomm_channel.hpp"
#include "timer.hpp" // Are nevoie de TimerDriver

class TimerHandler : public IEndpointHandler
{
	public:
	// Constructorul prime?te dependen?ele de care are nevoie
	TimerHandler(ICommChannel& comm, TimerDriver& timerDriver);

	bool Handle(const char* method, const char* path) override;

	private:
	// Func?ie ajut?toare pentru a trimite r?spunsuri
	void SendResponse(const char* code, const char* contentType, const char* body);

	ICommChannel& comm_;
	TimerDriver& timerDriver_;
};