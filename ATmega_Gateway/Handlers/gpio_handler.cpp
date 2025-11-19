#include "gpio_handler.hpp"
#include <string.h>
#include <stdio.h>
#include <stdlib.h> 

GpioHandler::GpioHandler(ICommChannel& comm)
: comm_(comm), gpio_(GpioDriver::GetInstance())
{
	gpio_.SetPinDirection(&DDRB, 5, PinDirection::OUTPUT);
	gpio_.SetPinLevel(&PORTB, 5, PinLevel::LOW); 
}


void GpioHandler::SendResponse(const char* code, const char* contentType, const char* body)
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


void GpioHandler::SendOK()
{
	SendResponse("200 OK", "text/plain", "OK");
}

bool GpioHandler::Handle(const char* method, const char* path)
{
	
	if (strcmp(method, "GET") != 0)
	{
		return false;
	}

	if (strncmp(path, "/gpio/toggle/", 13) == 0)
	{
		
		const char* restulCaii = path + 13; 
		char* portStr = strtok((char*)restulCaii, "/");
		char* pinStr = strtok(NULL, "/");
		
		if (portStr && pinStr && strcmp(portStr, "B") == 0)
		{
			uint8_t pin = atoi(pinStr); 
			gpio_.TogglePin(&PORTB, pin);
			SendOK();
			return true;
		}
	}

	
	if (strncmp(path, "/gpio/set/", 10) == 0)
	{
		
		const char* restulCaii = path + 10; 
		char* portStr = strtok((char*)restulCaii, "/");
		char* pinStr = strtok(NULL, "/");
		char* levelStr = strtok(NULL, "/");

		if (portStr && pinStr && levelStr && strcmp(portStr, "B") == 0)
		{
			uint8_t pin = atoi(pinStr);
			PinLevel level = (strcmp(levelStr, "high") == 0) ? PinLevel::HIGH : PinLevel::LOW;
			gpio_.SetPinLevel(&PORTB, pin, level);
			SendOK();
			return true;
		}
	}
	
	return false; 
}