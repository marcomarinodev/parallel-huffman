#ifndef FF_SOLUTION_H
#define FF_SOLUTION_H

#include "./utils.h"

#include <unordered_map>
#include <thread>

using namespace std;

namespace ff_solution
{
  
  /// @brief [FastFlow] Given a vector of characters, it returns a map where foreach
  /// character in chars there's a number representing the occurrences
  /// of that character using.
  /// @param chars vector of characters
  /// @return map of <character, occurrences>
  map<char, int> count_chars(vector<char> chars, int num_threads);


  /// @brief encode chars using huffman encoding algorithm
  /// @param encoding_table huffman encoding table
  /// @param chars input
  /// @param num_threads
  /// @return encoded input
  string encode(vector<char> chars, unordered_map<char, string> encoding_table, int num_threads);


  string compress(string encoded_string, int num_threads);
}

#endif