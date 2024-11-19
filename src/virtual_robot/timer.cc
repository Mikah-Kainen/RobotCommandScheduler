#include "../../inc/virtual_robot/timer.h"


Timer::Timer()
    : startTime{ std::chrono::system_clock::now() }
{
}

// void Timer::Start()
// {
//     startTime = std::chrono::system_clock::now();
// }

// void Timer::Stop()
// {
//     endTime = std::chrono::system_clock::now();
// }

// void Timer::Reset()
// {
//     startTime = std::chrono::system_clock::now();
// }

Timer& Timer::GetInstance()
{
    static Timer timer = Timer();
    return timer;
}

double Timer::ElapsedMilliseconds()
{
    std::chrono::time_point<std::chrono::system_clock> currentTime = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();
}

double Timer::ElapsedSeconds()
{
    return Timer::ElapsedMilliseconds() / 1000.0;
}