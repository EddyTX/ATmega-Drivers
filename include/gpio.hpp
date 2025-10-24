#ifndef ATMEGADRIVERS_GPIO_HPP_INCLUDED
#define ATMEGADRIVERS_GPIO_HPP_INCLUDED

// template pentru include guards: #include PROJECTNAME_FILENAME_HPP_INCLUDED

#include <stdint.h>


constexpr uint8_t MAX_PINS_PER_PORT = 7;

enum PinLevel : uint8_t 
{
	LOW = 0,
	HIGH = 1
};

enum PinDirection : uint8_t {
	INPUT = 0,
	OUTPUT = 1
};


enum GpioStatus : int32_t {
	OK = 0,
	INVALID_PORT = -1,
	INVALID_PIN = -2,
	NULL_POINTER = -3
};

class GpioStatusReturn
{
	
	public:
		GpioStatus GetStatus();	
	private:
		GpioStatus m_gpioStatus;
		uint8_t bytesRead;
	};

class GpioDriver 
{
	public:
		GpioDriver()
		{
			
		}

		GpioStatus SetPinDirection(volatile uint8_t* ddr, uint8_t pin, PinDirection direction);
		GpioStatus SetPinLevel(volatile uint8_t* port, uint8_t pin, PinLevel level);
		GpioStatus ReadPinLevel(volatile uint8_t* pinReg, uint8_t pin, PinLevel& level);
};

#endif /* GPIO_H_ */
