#include "timer.hpp"

static volatile uint32_t g_systemTicks = 0;
TimerDriver timerDriver;

ISR(TIMER2_COMPA_vect)
{
	g_systemTicks++;
	timerDriver.HandleISR();
}

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

void Timer::Increment()
{
	if(running)
	{
		elapsedMs++;
	}
}

bool Timer::HasElapsed()
{
	if(elapsedMs >= intervalMs)
	{
		elapsedMs = 0;
		return true;
	}
	return false;
}

TimerDriver::TimerDriver()
{
	for(uint8_t i = 0; i < MAX_TIMERS; i++)
	{
		callbacks[i] = nullptr;
	}

	TCCR2A = (1 << WGM21);
	TCCR2B = (1 << CS22) | (1 << CS20);
	OCR2A = 249;
	TIMSK2 = (1 << OCIE2A);
	sei();
}

int8_t TimerDriver::CreateTimer(uint32_t intervalMs)
{
	for(uint8_t i = 0; i < MAX_TIMERS; i++)
	{
		if(timers[i].GetInterval() == 0)
		{
			timers[i].SetInterval(intervalMs);
			return i;
		}
	}
	return -1;
}

TimerStatus TimerDriver::AttachCallback(uint8_t timerId, void (*callback)())
{
	if(timerId >= MAX_TIMERS)
	{
		return INVALID_TIMER;
	}
	if(callback == nullptr)
	{
		return NULL_CALLBACK;
	}
	callbacks[timerId] = callback;
	return TIMER_OK;
}

TimerStatus TimerDriver::DetachCallback(uint8_t timerId)
{
	if(timerId >= MAX_TIMERS)
	{
		return INVALID_TIMER;
	}
	callbacks[timerId] = nullptr;
	return TIMER_OK;
}

TimerStatus TimerDriver::StartTimer(uint8_t timerId)
{
	if(timerId >= MAX_TIMERS)
	{
		return INVALID_TIMER;
	}
	if(timers[timerId].IsRunning())
	{
		return TIMER_ALREADY_RUNNING;
	}
	timers[timerId].SetRunning(true);
	return TIMER_OK;
}

TimerStatus TimerDriver::StopTimer(uint8_t timerId)
{
	if(timerId >= MAX_TIMERS)
	{
		return INVALID_TIMER;
	}
	if(!timers[timerId].IsRunning())
	{
		return TIMER_NOT_RUNNING;
	}
	timers[timerId].SetRunning(false);
	timers[timerId].SetElapsed(0);
	return TIMER_OK;
}

void TimerDriver::HandleISR()
{
	for(uint8_t i = 0; i < MAX_TIMERS; i++)
	{
		timers[i].Increment();
		if(timers[i].HasElapsed() && callbacks[i])
		{
			callbacks[i]();
		}
	}
}
