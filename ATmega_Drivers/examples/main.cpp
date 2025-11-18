#include "project_defines.hpp"
#include "serial_channel.hpp"
#include "http_server.hpp"
#include "timer.hpp"
#include "gpio.hpp"
#include <string.h>
#include <avr/interrupt.h>

#include "timer_handler.hpp"
#include "gpio_handler.hpp"
#include "adc_handler.hpp"

SerialChannel comm_channel;
HttpServer server(comm_channel); 


TimerHandler timer_handler(comm_channel, timerDriver);
GpioHandler gpio_handler(comm_channel);
AdcHandler adc_handler(comm_channel);

int main()
{
	comm_channel.Init(9600);
	sei(); 
	
	
	server.RegisterHandler(&timer_handler);
	server.RegisterHandler(&gpio_handler);
	server.RegisterHandler(&adc_handler);

	const char* startup_msg = "\r\n--- Modular HTTP Server Started ---\r\n";
	comm_channel.SendData((const uint8_t*)startup_msg, strlen(startup_msg));

	while (1)
	{
		server.Process(); 
		timerDriver.Run(); 
	}
	
	return 0;
}