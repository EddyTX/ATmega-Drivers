#include "../include/gpio_handler.hpp"
#include <string.h>
#include <stdio.h>
#include <stdlib.h> // Pentru atoi

GpioHandler::GpioHandler(ICommChannel& comm)
: comm_(comm), gpio_(GpioDriver::GetInstance())
{
	// Exemplu: Set?m pinul 5 al portului B ca ie?ire (LED-ul de pe Arduino Uno/Nano)
	// Acesta este Pinul Digital 13 pe placu??
	gpio_.SetPinDirection(&DDRB, 5, PinDirection::OUTPUT);
	gpio_.SetPinLevel(&PORTB, 5, PinLevel::LOW); // Începe stins
}

// Func?ie ajut?toare (duplicat?)
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

// O func?ie ?i mai specific?
void GpioHandler::SendOK()
{
	SendResponse("200 OK", "text/plain", "OK");
}

bool GpioHandler::Handle(const char* method, const char* path)
{
	// Acest handler gestioneaz? doar metoda GET
	if (strcmp(method, "GET") != 0)
	{
		return false;
	}

	// Vom parsa c?i de genul:
	// GET /gpio/toggle/B/5  (Port B, Pin 5 -> LED-ul Arduino)
	// GET /gpio/set/B/5/high
	// GET /gpio/set/B/5/low

	// --- Endpoint 1: Toggle ---
	if (strncmp(path, "/gpio/toggle/", 13) == 0)
	{
		// Calea este "/gpio/toggle/B/5"
		const char* restulCaii = path + 13; // -> "B/5"
		char* portStr = strtok((char*)restulCaii, "/");
		char* pinStr = strtok(NULL, "/");
		
		// Deocamdat? accept?m doar Port B pentru simplitate
		if (portStr && pinStr && strcmp(portStr, "B") == 0)
		{
			uint8_t pin = atoi(pinStr); // Convertim "5" în 5
			gpio_.TogglePin(&PORTB, pin);
			SendOK();
			return true;
		}
	}

	// --- Endpoint 2: Set (Varietate) ---
	if (strncmp(path, "/gpio/set/", 10) == 0)
	{
		// Calea este "/gpio/set/B/5/high"
		const char* restulCaii = path + 10; // -> "B/5/high"
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
	
	return false; // Nu am gestionat nimic
}