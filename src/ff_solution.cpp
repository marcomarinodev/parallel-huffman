#include "../includes/ff_solution.h"

using namespace std;

map<char, int> ff_solution::count_chars(vector<char> chars, int num_threads)
{
  vector<map<char, int>> par_map_chars_chunks(num_threads);
  map<char, int> chars_counts;
  ff::ParallelFor count_par_for(num_threads);
  int chunk_size = chars.size() / num_threads;

  long ff_par_count_elapsed;
  {
    utimer ff_par_count_timer("ff par count", &ff_par_count_elapsed);
    count_par_for.parallel_for_static(
        0, num_threads, 1, 0,
        [chunk_size, num_threads, &par_map_chars_chunks, chars](const long i)
        {
          int start = i * chunk_size;
          int end = i == num_threads - 1 ? chars.size() : start + chunk_size;

          map<char, int> local_map_chars;

          for (int j = start; j < end; j++)
          {
            local_map_chars[chars[j]]++;
          }

          par_map_chars_chunks[i] = local_map_chars;
        });

cout << "here" << endl;

    // merging
    for (auto map_chars_chunk : par_map_chars_chunks)
      for (auto [character, occurrences] : map_chars_chunk)
        chars_counts[character] += occurrences;
  }

#ifdef DEBUG
  cout << "[FF] par count elapsed: " << ff_par_count_elapsed << endl;
#endif

  return chars_counts;
}

string ff_solution::encode(vector<char> chars, unordered_map<char, string> encoding_table, int num_threads)
{
  vector<string> encoded_chunks(num_threads);
  string result;
  ff::ParallelFor pf(num_threads);
  int chunk_size = chars.size() / num_threads;

  long ff_par_encoding_elapsed;
  {
    utimer ff_par_encoding("ff par encoding", &ff_par_encoding_elapsed);

    pf.parallel_for_static(
        0, num_threads, 1, 0, [chunk_size, num_threads, encoding_table, chars, &encoded_chunks](const long i)
        { 
          int start = i * chunk_size;
          int end = i == num_threads - 1 ? chars.size() : start + chunk_size;

          string local_result;

          for (int j = start; j < end; j++)
          {
            try
            {
                local_result += encoding_table.at(chars[j]);
            }
            catch (const out_of_range &e)
            {
                cerr << "[FF] Exception at " << e.what() << endl;
            }
          }
           
          encoded_chunks[i] = local_result; });

    // merging
    for (auto encoded_chunk : encoded_chunks)
      result += encoded_chunk;
  }

#ifdef DEBUG
  cout << "[FF] par encoding elapsed: " << ff_par_encoding_elapsed << endl;
#endif
  return result;
}