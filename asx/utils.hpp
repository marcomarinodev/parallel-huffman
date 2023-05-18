//
// Created by marco.marino on 5/16/2023.
//


#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <iomanip>
#include <chrono>
#include <fstream>
#include <vector>

#define START(timename) auto timename = std::chrono::system_clock::now();
#define STOP(timename,elapsed)  auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - timename).count();

void active_wait(int usecs);

class utimer {
    std::chrono::system_clock::time_point start;
    std::chrono::system_clock::time_point stop;
    std::string message;
    using usecs = std::chrono::microseconds;
    using msecs = std::chrono::milliseconds;

private:
    long * us_elapsed;

public:

    utimer(const std::string m) : message(m),us_elapsed((long *)NULL) {
        start = std::chrono::system_clock::now();
    }

    utimer(const std::string m, long * us) : message(m),us_elapsed(us) {
        start = std::chrono::system_clock::now();
    }

    ~utimer() {
        stop =
                std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed =
                stop - start;
        auto musec =
                std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();

        if(us_elapsed == NULL)
            std::cout << message << " computed in " << std::setw(15) << musec << " usec "
                      << std::endl;
        if(us_elapsed != NULL)
            (*us_elapsed) = musec;
    }
};

std::vector<std::string> read_file(std::string file_path);

#endif
