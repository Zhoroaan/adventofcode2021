#pragma once
#include <chrono>
#include <iostream>

class Timer
{
public:
    Timer(const char* InName)
        : Name(InName)
    {
        startTime = std::chrono::high_resolution_clock::now();
    }

    ~Timer()
    {
        const auto endTime = std::chrono::high_resolution_clock::now();
        const auto time = endTime - startTime;
        std::cout << Name << " [time taken " << time.count() / (1000. * 1000.)  << "ms]" << std::endl;
    }

protected:
    const char* Name;
    std::chrono::steady_clock::time_point startTime;
};
