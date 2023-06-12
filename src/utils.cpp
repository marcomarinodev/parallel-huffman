//
// Created by marco.marino on 5/20/2023.
//

#include "../includes/utils.h"

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

std::vector<char> read_file(std::string file_path)
{
    std::vector<char> characters;
    std::ifstream input_file(file_path);
    
    if (input_file.is_open())
    {
        char c;
        while (input_file.get(c))
            characters.push_back(c);

        input_file.close();
        return characters;
    }

    std::cout << "Unable to open a file named " << file_path << std::endl;
    return characters;
}

template <typename VecType>
bool vec_contains(std::vector<VecType> v, VecType element)
{
    return std::find(v.begin(), v.end(), element) != v.end();
}

bool testEqualVectors(std::vector<int> vec1, std::vector<int> vec2)
{
    if (vec1.size() != vec2.size()) return false;

    for (size_t i = 0; i < vec1.size(); i++)
        if (vec1[i] != vec2[i]) return false;

    return true;
}

void print_ascii_html_value(char c)
{
    std::cout << "&#" << std::dec << static_cast<int>(c) << ";";
}
