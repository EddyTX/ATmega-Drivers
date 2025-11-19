#ifndef ATMEGADRIVERS_ADC_HPP_INCLUDED
#define ATMEGADRIVERS_ADC_HPP_INCLUDED

#include <avr/io.h>
#include <stdint.h>

class AdcDriver
{
	public:
		static AdcDriver& GetInstance()
		{
			static AdcDriver instance;
			return instance;
		}

		void Init();
	
		uint16_t Read(uint8_t channel);

	private:
		AdcDriver() {}
		~AdcDriver() {}
		AdcDriver(const AdcDriver&) = delete;
		AdcDriver& operator=(const AdcDriver&) = delete;
};

#endif /* ATMEGADRIVERS_ADC_HPP_INCLUDED */