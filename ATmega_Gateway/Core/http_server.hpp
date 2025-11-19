#pragma once

#include "icomm_channel.hpp"
#include "iendpoint_handler.hpp"
#include "project_defines.hpp"

class HttpServer
{
	public:

		HttpServer(ICommChannel& comm);

		void Process();

		bool RegisterHandler(IEndpointHandler* handler);

	private:
		void HandleRequest(char* req);
		void SendResponse(const char* code, const char* contentType, const char* body);
		void Send404();
		void Send400();
		void Send403();

	ICommChannel& comm_;

	IEndpointHandler* handlers_[MAX_HANDLERS];
	uint8_t handlerCount_;
};