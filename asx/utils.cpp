//
// Created by marco.marino on 5/16/2023.
//
#include "utils.hpp"

void active_wait(int usecs)
{
    auto start = std::chrono::high_resolution_clock::now();
    auto end = false;
    while (!end)
    {
        auto elapsed = std::chrono::high_resolution_clock::now() - start;
        auto usec = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
        if (usec > usecs) end = true;
    }
}