#include "../include/http_server.hpp"
#include "../include/project_defines.hpp" // Pentru ALLOWED_HOST
#include <string.h>
#include <stdio.h>
#include <stdlib.h> // Pentru strtok

// Constructorul
HttpServer::HttpServer(ICommChannel& comm)
: comm_(comm), handlerCount_(0)
{
}

// Înregistrarea Handlerelor
bool HttpServer::RegisterHandler(IEndpointHandler* handler)
{
	if (handlerCount_ < MAX_HANDLERS)
	{
		handlers_[handlerCount_++] = handler;
		return true;
	}
	return false;
}

// Func?ie NOU?: Trimite eroare de securitate
void HttpServer::Send403()
{
	SendResponse("403 Forbidden", "text/plain", "Access Denied: Invalid Host Header");
}

// Logica principal? de procesare ?i securitate
void HttpServer::Process()
{
	static char buffer[HTTP_REQUEST_BUFFER_SIZE];
	static uint16_t index = 0;

	uint8_t data;
	while(comm_.ReceiveData(data))
	{
		char c = static_cast<char>(data);

		// Protec?ie la Buffer Overflow
		if(index < HTTP_REQUEST_BUFFER_SIZE - 1)
		{
			buffer[index++] = c;
			buffer[index] = 0; // Null terminate mereu
		}
		else
		{
			index = 0; // Reset for?at dac? e prea lung
		}

		// Verific?m finalul de request (linia goal? "\r\n\r\n")
		// Asta marcheaz? sfâr?itul headerelor HTTP conform RFC 7230
		if (index > 4 &&
		buffer[index-4] == '\r' && buffer[index-3] == '\n' &&
		buffer[index-2] == '\r' && buffer[index-1] == '\n')
		{
			// === ZONA DE SECURITATE ===
			
			// 1. Construim string-ul a?teptat: "Host: Siemens-Console"
			char expectedHost[64];
			snprintf(expectedHost, sizeof(expectedHost), "Host: %s", ALLOWED_HOST);

			// 2. C?ut?m acest text în tot buffer-ul primit
			if (strstr(buffer, expectedHost) != nullptr)
			{
				// ? G?SIT! Header-ul este corect. Proces?m cererea.
				HandleRequest(buffer);
			}
			else
			{
				// ? NU E G?SIT! Respingem conexiunea.
				Send403();
			}
			
			index = 0; // Reset?m bufferul pentru urm?torul request
		}
	}
}

// --- Func?iile ajut?toare pentru r?spunsuri ---

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

// --- Rutarea Cererii ---
void HttpServer::HandleRequest(char* req)
{
	// Pasul 1: Pars?m request-ul folosind strtok
	// (Deoarece am trecut de check-ul de securitate, ?tim c? e safe s? proces?m)
	char* method = strtok(req, " ");
	char* path = strtok(NULL, " ");
	char* version = strtok(NULL, " \r\n");

	if (method == nullptr || path == nullptr || version == nullptr)
	{
		Send400(); return;
	}

	// Pasul 2: Verific?m versiunea
	if (strcmp(version, "HTTP/1.1") != 0)
	{
		Send400(); return;
	}

	// Pasul 3: Deleg?m cererea c?tre Handlere
	for (uint8_t i = 0; i < handlerCount_; i++)
	{
		if (handlers_[i]->Handle(method, path))
		{
			return; // Handler-ul a gestionat cererea
		}
	}

	Send404();
}