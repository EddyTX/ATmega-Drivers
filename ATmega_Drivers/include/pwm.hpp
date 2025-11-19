#ifndef ATMEGADRIVERS_PWM_HPP_INCLUDED
#define ATMEGADRIVERS_PWM_HPP_INCLUDED

#include <avr/io.h>
#include <stdint.h>

class PwmDriver
{
	public:
	static PwmDriver& GetInstance()
	{
		static PwmDriver instance;
		return instance;
	}

	// Configureaz? Timer0 pentru Fast PWM pe PD6 (Ch0) ?i PD5 (Ch1)
	void Init();

	// channel: 0 (PD6) sau 1 (PD5)
	// duty: 0-255
	void SetDutyCycle(uint8_t channel, uint8_t duty);

	private:
	PwmDriver() {}
	~PwmDriver() {}
	PwmDriver(const PwmDriver&) = delete;
	PwmDriver& operator=(const PwmDriver&) = delete;
};

#endif