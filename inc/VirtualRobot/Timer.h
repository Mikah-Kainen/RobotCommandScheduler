#pragma once
#include <chrono>
#include <ctime>

class Timer
{
private:
    std::chrono::time_point<std::chrono::system_clock> startTime;
    //std::chrono::time_point<std::chrono::system_clock> endTime;

    Timer();

    // void Start();

    // void Stop();

    // void Reset();

public:

    static Timer& GetInstance();

    double ElapsedMilliseconds();

    double ElapsedSeconds();
};