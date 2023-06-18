//
// Created by marco.marino on 5/20/2023.
//

#ifndef PARALLEL_DISTRIB_COMPUTING_UTILS_H
#define PARALLEL_DISTRIB_COMPUTING_UTILS_H

#include <iostream>
#include <iomanip>
#include <chrono>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cerrno>
#include <cstring>
#include <mutex>
#include <future>
#include <filesystem>
#include <list>
#include <bitset>

#define START(timename) auto timename = std::chrono::system_clock::now();
#define STOP(timename, elapsed) auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - timename).count();

class utimer
{
    std::chrono::system_clock::time_point start;
    std::chrono::system_clock::time_point stop;
    std::string message;
    using usecs = std::chrono::microseconds;
    using msecs = std::chrono::milliseconds;

private:
    long *us_elapsed;

public:
    utimer(const std::string m) : message(m), us_elapsed((long *)NULL)
    {
        start = std::chrono::system_clock::now();
    }

    utimer(const std::string m, long *us) : message(m), us_elapsed(us)
    {
        start = std::chrono::system_clock::now();
    }

    ~utimer()
    {
        stop =
            std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed =
            stop - start;
        auto musec =
            std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();

        if (us_elapsed == NULL)
            std::cout << message << " computed in " << std::setw(15) << musec << " usec "
                      << std::endl;
        if (us_elapsed != NULL)
            (*us_elapsed) = musec;
    }
};

std::vector<char> par_read_file(const std::string& filename, int num_threads);

void par_write_file_string(const std::string &filename, const std::string &data, int num_threads);

std::vector<std::vector<std::bitset<8>>> par_compress(const std::vector<char> &chars, const std::unordered_map<char, std::string> &encoding_table);

void par_write_bits_chunks(std::vector<std::vector<std::bitset<8>>> compressed_chunks, const std::string &output_file);

void seq_write_bits_chunks(std::vector<std::vector<std::bitset<8>>> compressed_chunks, const std::string &output_file);

void active_wait(int usecs);

std::vector<char> read_file(std::string file_path);

template <typename VecType>
bool vec_contains(std::vector<VecType> v, VecType element);

void print_ascii_html_value(char c);

std::string vec_to_string(std::vector<char> vec);

void print_string_vector(std::vector<std::string> vec);

#endif // PARALLEL_DISTRIB_COMPUTING_UTILS_H
