#include <avr/io.h>
#include "gpio.h"


GpioStatus GpioDriver::SetPinDirection(volatile uint8_t* ddr, uint8_t pin, PinDirection dir)
{
	if (ddr == nullptr)
	{
		return GpioStatus::NULL_POINTER;
	}

	if (pin > MAX_PINS_PER_PORT)
	{
		return GpioStatus::INVALID_PIN;
	}

	if (PinDirection::OUTPUT == dir)
	{
		*ddr |= (1 << pin);
	}
	else
	{
		*ddr &= ~(1 << pin);
	}

	return GpioStatus::OK;
}

GpioStatus GpioDriver::SetPinLevel(volatile uint8_t* port, uint8_t pin, PinLevel level)
{
	if (port == nullptr)
	{
		return GpioStatus::NULL_POINTER;
	}

	if (pin > MAX_PINS_PER_PORT)
	{
		return GpioStatus::INVALID_PIN;
	}

	if (PinLevel::HIGH == level)
	{
		*port |= (1 << pin);
	}
	else
	{
		*port &= ~(1 << pin);
	}

	return GpioStatus::OK;
}

GpioStatus GpioDriver::ReadPinLevel(volatile uint8_t* pinReg, uint8_t pin, PinLevel& level)
{
	if (pinReg == nullptr)
	{
		return GpioStatus::NULL_POINTER;
	}

	if (pin > MAX_PINS_PER_PORT)
	{
		return GpioStatus::INVALID_PIN;
	}

	if (*pinReg & (1 << pin))
	{
		level = PinLevel::HIGH;
	}
	else
	{
		level = PinLevel::LOW;
	}

	return GpioStatus::OK;
}
