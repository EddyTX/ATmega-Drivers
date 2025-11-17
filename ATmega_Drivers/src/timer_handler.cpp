#include "../include/timer_handler.hpp"
#include <string.h>
#include <stdio.h>

TimerHandler::TimerHandler(ICommChannel& comm, TimerDriver& timerDriver)
: comm_(comm), timerDriver_(timerDriver)
{
}

// Func?ie ajut?toare (este duplicat?, o po?i muta într-o clas? HttpResponder)
void TimerHandler::SendResponse(const char* code, const char* contentType, const char* body)
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


bool TimerHandler::Handle(const char* method, const char* path)
{
	// Acest handler gestioneaz? doar metoda GET
	if (strcmp(method, "GET") != 0)
	{
		return false;
	}

	// Verific?m dac? este pentru noi
	if (strcmp(path, "/timer/elapsed") == 0)
	{
		uint32_t seconds = timerDriver_.GetSystemSeconds();
		char body[20];
		snprintf(body, sizeof(body), "%lu", seconds);
		SendResponse("200 OK", "text/plain", body);
		return true; // Am gestionat cererea
	}
	
	if (strcmp(path, "/msg") == 0)
	{
		SendResponse("200 OK", "text/html", "Hello World Modular");
		return true; // Am gestionat cererea
	}
	
	return false; // Nu este o cerere pentru noi
}