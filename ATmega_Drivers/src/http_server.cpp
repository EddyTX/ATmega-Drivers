#include "../include/http_server.hpp"
#include <string.h>
#include <stdio.h>
#include <stdlib.h> 


HttpServer::HttpServer(ICommChannel& comm)
: comm_(comm), handlerCount_(0)
{
}

bool HttpServer::RegisterHandler(IEndpointHandler* handler)
{
	if (handlerCount_ < MAX_HANDLERS)
	{
		handlers_[handlerCount_++] = handler;
		return true;
	}
	return false;
}

void HttpServer::Process()
{
	static char buffer[HTTP_REQUEST_BUFFER_SIZE];
	static uint16_t index = 0;

	uint8_t data;
	while(comm_.ReceiveData(data))
	{
		char c = static_cast<char>(data);

		if(c == '\n' || index >= sizeof(buffer) - 1)
		{
			buffer[index] = 0;
			HandleRequest(buffer); 
			index = 0;
		}
		else if(c != '\r')
		{
			buffer[index++] = c;
		}
	}
}


void HttpServer::SendResponse(const char* code, const char* contentType, const char* body)
{
	char response[128]; 
	int bodyLen = strlen(body);

	snprintf(response, sizeof(response),
	"HTTP/1.1 %s\r\n"
	"Content-Type: %s\r\n"
	"Content-Length: %d\r\n\r\n",
	code, contentType, bodyLen);

	comm_.SendData((const uint8_t*)response, strlen(response));
	comm_.SendData((const uint8_t*)body, bodyLen);
	comm_.SendData((const uint8_t*)"\r\n", 2); 
}

void HttpServer::Send404()
{
	SendResponse("404 Not Found", "text/plain", "Not Found");
}

void HttpServer::Send400()
{
	SendResponse("400 Bad Request", "text/plain", "Bad Request");
}



void HttpServer::HandleRequest(char* req)
{
	
	char* method = strtok(req, " ");
	char* path = strtok(NULL, " ");
	char* version = strtok(NULL, " \r\n"); 

	
	if (method == nullptr || path == nullptr || version == nullptr)
	{
		Send400(); 
		return;
	}

	
	if (strcmp(version, "HTTP/1.1") != 0)
	{
		Send400(); 
		return;
	}

	
	for (uint8_t i = 0; i < handlerCount_; i++)
	{
		if (handlers_[i]->Handle(method, path))
		{
			return; 
		}
	}

	Send404();
}