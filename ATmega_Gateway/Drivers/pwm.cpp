#include "pwm.hpp"

void PwmDriver::Init()
{
	DDRD |= (1 << DDD6) | (1 << DDD5);

	TCCR0A |= (1 << COM0A1) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);

	TCCR0B |= (1 << CS01) | (1 << CS00);
	
	OCR0A = 0;
	OCR0B = 0;
}

void PwmDriver::SetDutyCycle(uint8_t channel, uint8_t duty)
{
	if (channel == 0)
	{
		OCR0A = duty;
	}
	else if (channel == 1)
	{
		OCR0B = duty;
	}
}