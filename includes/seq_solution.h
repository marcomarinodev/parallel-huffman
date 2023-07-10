#ifndef SEQ_SOLUTION_H
#define SEQ_SOLUTION_H

#include "./utils.h"

#include <unordered_map>
#include <thread>

using namespace std;

namespace seq_solution
{
  
  /// @brief [Monothread] Given a vector of characters, it returns a map where foreach
  /// character in chars there's a number representing the occurrences
  /// of that character using.
  /// @param chars vector of characters
  /// @return map of <character, occurrences>
  map<char, int> count_chars(const vector<char>& chars);

  /// @brief [Monothread] encode chars using huffman encoding algorithm
  /// @param encoding_table huffman encoding table
  /// @param chars input
  /// @return encoded input
  string encode(vector<char> chars, unordered_map<char, string> encoding_table);

  /// @brief [Monothread] compress an encoded string passed by parameter using bitsets
  /// @param encoded_string
  /// @return compressed encoded string as a vector of bytes
  vector<bitset<8>> compress(string encoded_string);

 }

#endif