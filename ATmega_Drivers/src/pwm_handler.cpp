#include "../include/pwm_handler.hpp"
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
	// R?spuns scurt ?i eficient
	const char* response = "HTTP/1.1 200 OK\r\nContent-Length: 2\r\n\r\nOK";
	comm_.SendData((const uint8_t*)response, strlen(response));
}

bool PwmHandler::Handle(const char* method, const char* path)
{
	// Industrial standard: Modific?rile se fac cu POST
	if (strcmp(method, "POST") != 0) return false;

	// Cale: /pwm/set/0/128
	if (strncmp(path, "/pwm/set/", 9) == 0)
	{
		// Pointer dup? "/pwm/set/" -> "0/128"
		char* params = (char*)(path + 9);
		
		// C?ut?m separatorul '/'
		char* slash = strchr(params, '/');
		
		if (slash)
		{
			// Spargem stringul temporar
			*slash = 0;
			
			uint8_t channel = atoi(params);      // "0"
			uint8_t duty = atoi(slash + 1);      // "128"
			
			// Repar?m stringul (op?ional, dar good practice)
			*slash = '/';
			
			pwm_.SetDutyCycle(channel, duty);
			SendOK();
			return true;
		}
	}

	return false;
}