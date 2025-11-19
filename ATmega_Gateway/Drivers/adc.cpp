#include "adc.hpp"

void AdcDriver::Init()
{
	ADMUX |= (1 << REFS0);
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	ADCSRA |= (1 << ADEN);
}

uint16_t AdcDriver::Read(uint8_t channel)
{
	channel &= 0x07;

	ADMUX &= 0xF0;
	ADMUX |= channel;

	ADCSRA |= (1 << ADSC);

	while (ADCSRA & (1 << ADSC));

	return ADC;
}