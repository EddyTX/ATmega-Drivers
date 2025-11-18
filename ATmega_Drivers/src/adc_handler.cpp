#include "../include/adc_handler.hpp"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

AdcHandler::AdcHandler(ICommChannel& comm)
: comm_(comm), adc_(AdcDriver::GetInstance())
{
	adc_.Init();
}

void AdcHandler::SendResponse(const char* code, const char* contentType, const char* body)
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

bool AdcHandler::Handle(const char* method, const char* path)
{
	if (strcmp(method, "GET") != 0)
	{
		return false;
	}

	if (strncmp(path, "/adc/read/", 10) == 0)
	{
		uint8_t channel = atoi(path + 10);

		uint16_t val = adc_.Read(channel);

		char body[10];
		snprintf(body, sizeof(body), "%d", val);

		SendResponse("200 OK", "text/plain", body);
		return true;
	}

	return false;
}