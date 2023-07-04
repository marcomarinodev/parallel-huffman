#ifndef NT_SOLUTION_H
#define NT_SOLUTION_H

#define MAP_NW 2
#define RED_NW 2

#include "./utils.h"

#include <thread>

using namespace std;

namespace nt_solution
{

  /// @brief [Multiple Threads] Given a vector of characters, it returns a map where foreach
  /// character in chars there's a number representing the occurrences
  /// of that character.
  /// @param chars vector of characters
  /// @return map of <character, occurrences>
  map<char, int> count_chars(const vector<char>& chars, int num_threads);

  class Gmr
  {
    public:
    int map_nw = MAP_NW;
    int red_nw = RED_NW;

    vector<map<char, int>> mappers_outputs;
    vector<vector<pair<char, int>>> reducers_queue;
    vector<mutex> reducers_queue_mutexes;
    mutex reducers_result_mutex;
    map<char, int> reducers_result;

    Gmr(int map_nw, int red_nw);

    /// @brief [GMR] count the characters passed in input
    /// @param input passed input vector of characters
    /// @return map where for each character you have its occurrence
    map<char, int> count_chars(vector<char> input);
  };

  /// @brief [Multiple Threads] encoding phase
  /// @param chars input characters
  /// @param encoding_table huffman encoding table
  /// @return string version of the input encoding
  string encode(const vector<char>& chars, const unordered_map<char, string> &encoding_table, int num_threads);

}

#endif