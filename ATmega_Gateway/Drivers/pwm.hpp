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

	void Init();

	void SetDutyCycle(uint8_t channel, uint8_t duty);

	private:
	PwmDriver() {}
	~PwmDriver() {}
	PwmDriver(const PwmDriver&) = delete;
	PwmDriver& operator=(const PwmDriver&) = delete;
};

#endif