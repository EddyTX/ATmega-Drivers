#ifndef ATMEGADRIVERS_TIMER_HPP_INCLUDED
#define ATMEGADRIVERS_TIMER_HPP_INCLUDED

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

constexpr uint8_t MAX_TIMERS = 3;

enum TimerStatus : int32_t
{
	TIMER_OK = 0,
	INVALID_TIMER = -1,
	NULL_CALLBACK = -2,
	TIMER_ALREADY_RUNNING = -3,
	TIMER_NOT_RUNNING = -4
};

class Timer
{
	public:
		Timer(uint32_t interval = 0);
	
		void SetInterval(uint32_t interval);
		uint32_t GetInterval() const;
	
		void SetElapsed(uint32_t elapsed);
		uint32_t GetElapsed() const;
	
		void SetRunning(bool state);
		bool IsRunning() const;
	
		void Increment();
		bool HasElapsed();

	private:
		uint32_t intervalMs;
		uint32_t elapsedMs;
		bool running;
};

class TimerDriver
{
	public:
		TimerDriver();
	
		int8_t CreateTimer(uint32_t intervalMs);
		TimerStatus AttachCallback(uint8_t timerId, void (*callback)());
		TimerStatus DetachCallback(uint8_t timerId);
	
		TimerStatus StartTimer(uint8_t timerId);
		TimerStatus StopTimer(uint8_t timerId);

		void HandleISR();

	private:
		Timer timers[MAX_TIMERS];
		void (*callbacks[MAX_TIMERS])();
};

extern TimerDriver timerDriver;

#endif /* ATMEGADRIVERS_TIMER_HPP_INCLUDED */
