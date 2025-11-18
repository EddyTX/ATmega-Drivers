#pragma once

#include "iendpoint_handler.hpp"
#include "icomm_channel.hpp"
#include "adc.hpp"

class AdcHandler : public IEndpointHandler
{
	public:
		AdcHandler(ICommChannel& comm);
		bool Handle(const char* method, const char* path) override;

	private:
		void SendResponse(const char* code, const char* contentType, const char* body);

		ICommChannel& comm_;
		AdcDriver& adc_;
};