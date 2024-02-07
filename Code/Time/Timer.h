#pragma once
#include <windows.h>

class Timer
{
public:
    Timer();
    void Start();
    void Stop();
    void Reset();
    double GetElapsedSeconds() const;

private:
    double SecondsPerTick;
    __int64 TimerStarted;
    __int64 PausedTime;
    __int64 TimerStopped;
    __int64 PreviousTimeFrame;
    __int64 CurrentTime;
    bool Stopped;
};