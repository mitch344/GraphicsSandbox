#include "Timer.h"

Timer::Timer() : SecondsPerTick(0.0), TimerStarted(0),
PausedTime(0), TimerStopped(0), PreviousTimeFrame(0), CurrentTime(0), Stopped(false)
{
    __int64 countsPerSec;
    QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
    SecondsPerTick = 1.0 / static_cast<double>(countsPerSec);
}

void Timer::Start()
{
    if (Stopped)
    {
        __int64 startTime;
        QueryPerformanceCounter((LARGE_INTEGER*)&startTime);
        if (Stopped)
        {
            PausedTime += (startTime - TimerStopped);
            PreviousTimeFrame = startTime;
            TimerStopped = 0;
            Stopped = false;
        }
    }
}

void Timer::Stop()
{
    if (!Stopped)
    {
        __int64 currTime;
        QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
        TimerStopped = currTime;
        Stopped = true;
    }
}

void Timer::Reset()
{
    __int64 currTime;
    QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
    TimerStarted = currTime;
    PreviousTimeFrame = currTime;
    TimerStopped = 0;
    PausedTime = 0;
    Stopped = false;
}

double Timer::GetElapsedSeconds() const
{
    __int64 currTime;
    QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

    if (Stopped)
    {
        return static_cast<double>(((TimerStopped - PausedTime) - TimerStarted) * SecondsPerTick);
    }
    else
    {
        return static_cast<double>(((currTime - PausedTime) - TimerStarted) * SecondsPerTick);
    }
}