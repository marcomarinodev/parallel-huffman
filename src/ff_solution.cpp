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

  int remainder = result.length() % 8;

  if (remainder != 0)
      result.append(8 - remainder, '0');

  return result;
}

string ff_solution::compress(string encoded_string, int num_threads)
{
  vector<string> compressed_chunks(num_threads);
  string result;
  ff::ParallelFor pf(num_threads);
  int chunk_size = encoded_string.length() / num_threads;
  chunk_size = chunk_size - chunk_size % 8;

  pf.parallel_for_static(
    0, num_threads, 1, 0, [chunk_size, num_threads, &compressed_chunks, encoded_string](const long i)
  {
    int start = i * chunk_size;
    int end = i == num_threads - 1 ? encoded_string.length() : start + chunk_size;
    string compressed_chunk;

    for (int j = start; j < end; j += 8)
    {
      bitset<8> bits(encoded_string.substr(j, 8));
      compressed_chunk += char(bits.to_ulong());
    }

    compressed_chunks[i] = compressed_chunk;
  });

  // merging
  for (int i = 0; i < num_threads; i++)
    result += compressed_chunks[i];

  return result;
}