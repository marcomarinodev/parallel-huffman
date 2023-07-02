#ifndef FF_SOLUTION_H
#define FF_SOLUTION_H

#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>
#include <string>
#include <thread>
#include <ff/ff.hpp>
#include <ff/parallel_for.hpp>

#include "./utils.h"

using namespace std;

namespace ff_solution
{
  
  /// @brief [FastFlow] Given a vector of characters, it returns a map where foreach
  /// character in chars there's a number representing the occurrences
  /// of that character using.
  /// @param chars vector of characters
  /// @return map of <character, occurrences>
  map<char, int> count_chars(vector<char> chars);


  /// @brief encode chars using huffman encoding algorithm
  /// @param encoding_table huffman encoding table
  /// @param chars input
  /// @param num_threads
  /// @return encoded input
  string encode(vector<char> chars, unordered_map<char, string> encoding_table, int num_threads);
}

#endif