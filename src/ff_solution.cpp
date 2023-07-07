#include "../includes/ff_solution.h"

using namespace std;

map<char, int> ff_solution::count_chars(vector<char> chars, int num_threads)
{
  map<char, int> par_map_chars;

  // parallel counting
  ff::ParallelForReduce<map<char, int>> count_par_for;

  long ff_par_count_elapsed;
  {
    utimer ff_par_count_timer("ff par count", &ff_par_count_elapsed);
    count_par_for.parallel_reduce(
        par_map_chars, map<char, int>(), 0, chars.size(), 1,
        [&](const long i, map<char, int> &sub_result)
        {
          sub_result[chars[i]]++;
        },
        [](map<char, int> &result, const map<char, int> &sub_result)
        {
          for (const auto &[key, value] : sub_result)
            result[key] += value;
        },
        num_threads);
  }

#ifdef DEBUG
  cout << "[FF] par count elapsed: " << ff_par_count_elapsed << endl;
#endif

  return par_map_chars;
}

string ff_solution::encode(vector<char> chars, unordered_map<char, string> encoding_table, int num_threads)
{
  string result;
  ff::ParallelForReduce<string> pr(num_threads);

  long ff_par_encoding_elapsed;
  {
    utimer ff_par_encoding("ff par encoding", &ff_par_encoding_elapsed);

    pr.parallel_reduce(
        result, string(), 0, chars.size(), 1, [&](const long i, string &partial_result)
        { partial_result += encoding_table[chars[i]]; },
        [](string &r, const string &partial_result)
        {
          r += partial_result;
        });
  }

#ifdef DEBUG
  cout << "[FF] par encoding elapsed: " << ff_par_encoding_elapsed << endl;
#endif
  return result;
}