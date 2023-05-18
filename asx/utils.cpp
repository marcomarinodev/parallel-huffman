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

std::vector<std::string> read_file(std::string file_path)
{
    std::ifstream file(file_path);
    std::vector<std::string> words;

    if (file.is_open())
    {
        std::string word;
        while (file >> word)
            words.push_back(word);

        file.close();
        return words;
    }

    std::cout << "Unable to open a file named " << file_path << std::endl;
    return words;
}