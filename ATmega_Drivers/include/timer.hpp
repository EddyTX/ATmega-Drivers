#ifndef ATMEGADRIVERS_TIMER_HPP_INCLUDED
#define ATMEGADRIVERS_TIMER_HPP_INCLUDED

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "project_defines.hpp"

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
	Timer(); 
	Timer(uint32_t interval); 
	void SetInterval(uint32_t interval);
	uint32_t GetInterval() const;
	void SetElapsed(uint32_t elapsed);
	uint32_t GetElapsed() const;
	void SetRunning(bool state);
	bool IsRunning() const;
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

	void Run();

	uint32_t GetSystemSeconds() const;
	
	private:
	Timer timers[TIMER_MAX_COUNT];
	void (*callbacks[TIMER_MAX_COUNT])() = {nullptr};
};
 extern TimerDriver timerDriver;

#endif 