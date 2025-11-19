#include "timer.hpp"
#include "project_defines.hpp"
#include <avr/interrupt.h>
#include <avr/io.h>


static volatile uint32_t g_systemTicks = 0;
TimerDriver timerDriver;


ISR(TIMER2_COMPA_vect)
{
	g_systemTicks++;
}


Timer::Timer(){}
Timer::Timer(uint32_t interval)
{
	intervalMs = interval;
	elapsedMs = 0;
	running = false;
}

void Timer::SetInterval(uint32_t interval)
{
	intervalMs = interval;
}

uint32_t Timer::GetInterval() const
{
	return intervalMs;
}

void Timer::SetElapsed(uint32_t elapsed)
{
	elapsedMs = elapsed;
}

uint32_t Timer::GetElapsed() const
{
	return elapsedMs;
}

void Timer::SetRunning(bool state)
{
	running = state;
}

bool Timer::IsRunning() const
{
	return running;
}

bool Timer::HasElapsed()
{
	if (running && elapsedMs >= intervalMs && intervalMs > 0)
	{
		elapsedMs = 0;
		return true;
	}
	return false;
}


uint32_t TimerDriver::GetSystemSeconds() const
{
	uint32_t ticks;
	
	uint8_t sreg = SREG;
	cli();
	ticks = g_systemTicks;
	SREG = sreg;
	
	return ticks / 1000;
}

TimerDriver::TimerDriver()
{
	
	for (uint8_t i = 0; i < TIMER_MAX_COUNT; i++)
	{
		callbacks[i] = nullptr;
	}

	
	OCR2A = 249; 
	TCCR2A = (1 << WGM21); 
	TCCR2B = (1 << CS21) | (1 << CS20); 
	TIMSK2 = (1 << OCIE2A); 
	sei(); 
}

int8_t TimerDriver::CreateTimer(uint32_t intervalMs)
{
	for (uint8_t i = 0; i < TIMER_MAX_COUNT; i++)
	{
		if (timers[i].GetInterval() == 0) 
		{
			timers[i].SetInterval(intervalMs);
			timers[i].SetElapsed(0);
			return i;
		}
	}
	return -1;
}

TimerStatus TimerDriver::AttachCallback(uint8_t timerId, void (*callback)())
{
	if (timerId >= TIMER_MAX_COUNT)
	{
		return INVALID_TIMER;
	}
	if (callback == nullptr)
	{
		return NULL_CALLBACK;
	}
	callbacks[timerId] = callback;
	return TIMER_OK;
}

TimerStatus TimerDriver::DetachCallback(uint8_t timerId)
{
	if (timerId >= TIMER_MAX_COUNT)
	{
		return INVALID_TIMER;
	}
	callbacks[timerId] = nullptr;
	return TIMER_OK;
}

TimerStatus TimerDriver::StartTimer(uint8_t timerId)
{
	if (timerId >= TIMER_MAX_COUNT)
	{
		return INVALID_TIMER;
	}
	if (timers[timerId].IsRunning())
	{
		return TIMER_ALREADY_RUNNING;
	}
	timers[timerId].SetRunning(true);
	timers[timerId].SetElapsed(0);
	return TIMER_OK;
}

TimerStatus TimerDriver::StopTimer(uint8_t timerId)
{
	if (timerId >= TIMER_MAX_COUNT)
	{
		return INVALID_TIMER;
	}
	if (!timers[timerId].IsRunning())
	{
		return TIMER_NOT_RUNNING;
	}
	timers[timerId].SetRunning(false);
	return TIMER_OK;
}

void TimerDriver::Run()
{
	static uint32_t lastTick = 0;

	uint32_t currentTicks = 0;

	uint8_t sreg = SREG;
	cli();
	currentTicks = g_systemTicks;
	SREG = sreg;

	if (currentTicks != lastTick)
	{
		uint32_t diff = currentTicks - lastTick;
		lastTick = currentTicks;

		for (uint8_t i = 0; i < TIMER_MAX_COUNT; i++)
		{
			if (timers[i].IsRunning())
			{
				timers[i].SetElapsed(timers[i].GetElapsed() + diff);

				if (timers[i].HasElapsed() && callbacks[i] != nullptr)
				{
					callbacks[i]();
				}
			}
		}
	}
}