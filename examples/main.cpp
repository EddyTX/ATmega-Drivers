#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "../include/timer.hpp"
#include "../include/gpio.hpp"


GpioDriver gpio;

void ToggleLed1()
{
	gpio.TogglePin(&PORTB, PINB0);
}

void ToggleLed2()
{
	gpio.TogglePin(&PORTB, PINB1);
}

int main()
{
	gpio.SetPinDirection(&DDRB, PINB0, PinDirection::OUTPUT);
	gpio.SetPinDirection(&DDRB, PINB1, PinDirection::OUTPUT);
	gpio.SetPinLevel(&DDRB, PINB0, PinLevel::HIGH);
	
	int8_t timer1 = timerDriver.CreateTimer(1000);
	int8_t timer2 = timerDriver.CreateTimer(2000);

	timerDriver.AttachCallback(timer1, ToggleLed1);
	timerDriver.AttachCallback(timer2, ToggleLed2);

	timerDriver.StartTimer(timer1);
	timerDriver.StartTimer(timer2);

	while(1)
	{
		// totul se face în ISR
	}
}
