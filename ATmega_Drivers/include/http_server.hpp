#pragma once

#include "icomm_channel.hpp"
#include "iendpoint_handler.hpp" // Includem noua interfa??
#include "project_defines.hpp"   // Pentru MAX_HANDLERS

class HttpServer
{
	public:
	// Constructorul prime?te doar dependen?ele de baz?
	HttpServer(ICommChannel& comm);

	void Process();

	// Metod? public? pentru a ad?uga handlere (din main.cpp)
	bool RegisterHandler(IEndpointHandler* handler);

	private:
	// Schimbat la char* (non-const) pentru a permite parsarea cu strtok
	void HandleRequest(char* req);

	// Func?ii ajut?toare pentru r?spunsuri
	void SendResponse(const char* code, const char* contentType, const char* body);
	void Send404();
	void Send400(); // Pentru Bad Request (lips? HTTP/1.1)

	ICommChannel& comm_;

	// Lista de handlere
	IEndpointHandler* handlers_[MAX_HANDLERS];
	uint8_t handlerCount_;
};