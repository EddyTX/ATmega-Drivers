#include "pwm_handler.hpp"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

PwmHandler::PwmHandler(ICommChannel& comm)
: comm_(comm), pwm_(PwmDriver::GetInstance())
{
	pwm_.Init();
}

void PwmHandler::SendOK()
{
	const char* response = "HTTP/1.1 200 OK\r\nContent-Length: 2\r\n\r\nOK";
	comm_.SendData((const uint8_t*)response, strlen(response));
}

bool PwmHandler::Handle(const char* method, const char* path)
{
	if (strcmp(method, "POST") != 0) return false;

	if (strncmp(path, "/pwm/set/", 9) == 0)
	{
		char* params = (char*)(path + 9);
		
		char* slash = strchr(params, '/');
		
		if (slash)
		{
			*slash = 0;
			
			uint8_t channel = atoi(params);
			uint8_t duty = atoi(slash + 1);
			
			*slash = '/';
			
			pwm_.SetDutyCycle(channel, duty);
			SendOK();
			return true;
		}
	}

	return false;
}