#include "../include/pwm.hpp"

void PwmDriver::Init()
{
	// 1. Configuram pinii PD6 si PD5 ca Iesire
	// PD6 = OC0A
	// PD5 = OC0B
	DDRD |= (1 << DDD6) | (1 << DDD5);

	// 2. Configuram Timer0 - Fast PWM Mode
	// WGM00=1, WGM01=1 -> Fast PWM
	// COM0A1=1 -> Clear OC0A on Compare Match (PWM pe PD6)
	// COM0B1=1 -> Clear OC0B on Compare Match (PWM pe PD5)
	TCCR0A |= (1 << COM0A1) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);

	// 3. Prescaler 64 (Frecventa ~976Hz)
	TCCR0B |= (1 << CS01) | (1 << CS00);
	
	// Pornim cu ele stinse
	OCR0A = 0;
	OCR0B = 0;
}

void PwmDriver::SetDutyCycle(uint8_t channel, uint8_t duty)
{
	if (channel == 0)
	{
		OCR0A = duty; // Pin 6
	}
	else if (channel == 1)
	{
		OCR0B = duty; // Pin 5
	}
}