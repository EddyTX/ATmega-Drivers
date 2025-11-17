#include "project_defines.hpp"
#include "serial_channel.hpp"
#include "http_server.hpp"
#include "timer.hpp"
#include "gpio.hpp"
#include <string.h>
#include <avr/interrupt.h>

// Includem noile fi?iere de handler
#include "timer_handler.hpp"
#include "gpio_handler.hpp"

// --- Cre?m instan?ele ---
SerialChannel comm_channel;
HttpServer server(comm_channel); // Serverul prime?te doar canalul de comm

// Cre?m handlerele, dându-le dependen?ele de care au nevoie
// (TimerDriver este o instan?? global? din timer.cpp)
TimerHandler timer_handler(comm_channel, timerDriver);
GpioHandler gpio_handler(comm_channel);

int main()
{
	comm_channel.Init(9600);
	sei(); // Activ?m întreruperile
	
	// --- Înregistr?m handlerele în server ---
	server.RegisterHandler(&timer_handler);
	server.RegisterHandler(&gpio_handler);

	const char* startup_msg = "\r\n--- Modular HTTP Server Started ---\r\n";
	comm_channel.SendData((const uint8_t*)startup_msg, strlen(startup_msg));

	while (1)
	{
		server.Process(); // Serverul ruleaz?
		timerDriver.Run(); // Timer-ul ruleaz?
	}
	
	return 0;
}