#include "../include/http_server.hpp"
#include <string.h>
#include <stdio.h>
#include <stdlib.h> // Pentru strtok

// Constructorul, acum mult mai simplu
HttpServer::HttpServer(ICommChannel& comm)
: comm_(comm), handlerCount_(0)
{
}

// Metoda de a ad?uga un handler în list?
bool HttpServer::RegisterHandler(IEndpointHandler* handler)
{
	if (handlerCount_ < MAX_HANDLERS)
	{
		handlers_[handlerCount_++] = handler;
		return true;
	}
	return false;
}

// Process() r?mâne la fel ca înainte (cite?te datele de pe serial?)
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
			HandleRequest(buffer); // Trimitem buffer-ul (non-const)
			index = 0;
		}
		else if(c != '\r')
		{
			buffer[index++] = c;
		}
	}
}

// --- Func?iile ajut?toare pentru r?spunsuri ---

void HttpServer::SendResponse(const char* code, const char* contentType, const char* body)
{
	char response[128]; // Buffer temporar pentru headere
	int bodyLen = strlen(body);

	snprintf(response, sizeof(response),
	"HTTP/1.1 %s\r\n"
	"Content-Type: %s\r\n"
	"Content-Length: %d\r\n\r\n",
	code, contentType, bodyLen);

	comm_.SendData((const uint8_t*)response, strlen(response));
	comm_.SendData((const uint8_t*)body, bodyLen);
	comm_.SendData((const uint8_t*)"\r\n", 2); // Final de r?spuns
}

void HttpServer::Send404()
{
	SendResponse("404 Not Found", "text/plain", "Not Found");
}

void HttpServer::Send400()
{
	SendResponse("400 Bad Request", "text/plain", "Bad Request");
}


// --- Logica principal? (refactorizat?) ---

void HttpServer::HandleRequest(char* req)
{
	// Pasul 1: Pars?m request-ul folosind strtok
	char* method = strtok(req, " ");
	char* path = strtok(NULL, " ");
	char* version = strtok(NULL, " \r\n"); // Elimin?m ?i \r

	// Verific?m dac? avem toate cele 3 componente
	if (method == nullptr || path == nullptr || version == nullptr)
	{
		Send400(); // Bad Request
		return;
	}

	// Pasul 2: Verific?m versiunea (a?a cum ai cerut)
	if (strcmp(version, "HTTP/1.1") != 0)
	{
		Send400(); // Bad Request
		return;
	}

	// Pasul 3: Deleg?m cererea
	// Întreb?m fiecare handler pe rând
	for (uint8_t i = 0; i < handlerCount_; i++)
	{
		if (handlers_[i]->Handle(method, path))
		{
			return; // Handler-ul a gestionat cererea, am terminat
		}
	}

	// Dac? am ajuns aici, niciun handler nu a recunoscut calea
	Send404();
}