#include "../includes/tests.h"
#include "../includes/asx1.h"
#include "../includes/asx2.h"
// #include "asx3.cpp"
#include "../includes/huffman.h"

#include <future>
#include <filesystem>
#include <list>
#include <bitset>
#include <condition_variable>
#include <unordered_map>

#include <algorithm>
#include <thread>

#include <ff/ff.hpp>
#include <ff/parallel_for.hpp>

using namespace std;

int main(int argc, char *argv[])
{

	bool perform_tests = argc > 1 ? argv[1] == "test" : false;

	vector<string> input_filenames;
	vector<long> elapsed_times;

	if (perform_tests) 
	{
		test_fastflow_encoding();
		test_native_threads_encoding();
		return 0;
	}

	input_filenames.push_back("./inputs/1k");
	input_filenames.push_back("./inputs/10k");
	input_filenames.push_back("./inputs/50k");
	input_filenames.push_back("./inputs/100k");
	input_filenames.push_back("./inputs/200k");

	for (auto input_filename : input_filenames)
	{
		cout << "\n========================" << endl;
		cout << "compressing: " << input_filename << endl;
		vector<char> chars;
		long total_elapsed = 0;

		// => FILE READING <=
		long seq_file_read_elapsed;
		{
			utimer seq_read("seq file read", &seq_file_read_elapsed);
			chars = read_file(input_filename);
		}
		cout << "seq file read computed in: " << seq_file_read_elapsed << endl;
		total_elapsed += seq_file_read_elapsed;

		bool do_parallel_policy = chars.size() > 15 * 1000;

		// => COUNTING CHARS <=
		map<char, int> par_map_chars, seq_map_chars;
		// [PAR]: parallel google map reduce to count the characters
		// int map_nw = thread::hardware_concurrency();
		// int red_nw = map_nw / 2;
		// Asx3 asx3(map_nw, red_nw);

		if (do_parallel_policy)
		{
			cout << "file big => parallel counting" << endl;
			long par_2threads_word_count_elapsed;
			{
				utimer par_2threads_words_count("par 2threads words count", &par_2threads_word_count_elapsed);
				// par_map_chars = asx3.par_gmr<char, int, char>(chars, map_nw, red_nw);
				// par_map_chars = par_count_chars(chars);
				ff::ParallelForReduce<map<char, int>> count_par_for;

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
						});
			}
			cout << "par 2threads words count computed in: " << par_2threads_word_count_elapsed << endl;
			total_elapsed += par_2threads_word_count_elapsed;
		}
		else
		{
			cout << "file small => sequential counting" << endl;
			/// [SEQ]: sequential counting characters
			long seq_words_count_elapsed;
			{
				utimer seq_words_count("seq words count", &seq_words_count_elapsed);
				seq_map_chars = count_chars(chars);
			}
			cout << "seq words count computed in: " << seq_words_count_elapsed << endl;
			total_elapsed += seq_words_count_elapsed;
		}

		// => ENCODING TABLE BUILDER <=

		// [SEQ]: this is sequential because the tree size is upper bounded
		// by the ASCII table size
		min_heap_node *huffman_tree;
		unordered_map<char, string> encoding_table;
		long build_encoding_table_elasped;
		{
			utimer t_build_encoding_table("build encoding table", &build_encoding_table_elasped);
			huffman_tree = build_huffman_tree(do_parallel_policy ? par_map_chars : seq_map_chars);
			encoding_table = build_encoding_table(huffman_tree);
		}
		cout << "build encoding table computed in: " << build_encoding_table_elasped << endl;
		total_elapsed += build_encoding_table_elasped;

		// => ENCODING <=
		if (do_parallel_policy)
		{
			cout << "file big => parallel encoding" << endl;
			// [PAR]: parallel encoding
			/*
			 *                        +-----------+
			 *                        |           |
			 *              +-------->| Encoder 1 +------+
			 *              |         |           |      |
			 *              |         +-----------+      |
			 *              |                            |
			 *              |              *             v
			 * ------------>|              *       [[0,1,...,], ..., [1,0,...,]]
			 *              |              *                              ^
			 *              |                                             |
			 *              |         +-----------+                       |
			 *              |         |           |                       |
			 *              +-------->| Encoder N +-----------------------+
			 *                        |           |
			 *                        +-----------+
			 */
			long par_encoding_elapsed;
			{
				utimer t_par_encode("par encoding", &par_encoding_elapsed);
				string par_encoded_string = par_encode(chars, encoding_table);
				vector<bitset<8>> par_encoded_bitset = produce_encoded_bitset(par_encoded_string);
			}
			cout << "par encoding computed in: " << par_encoding_elapsed << endl;
			total_elapsed += par_encoding_elapsed;
		}
		else
		{
			cout << "file small => sequential encoding" << endl;
			// [SEQ]: sequential encoding
			long seq_encoding_elapsed;
			{
				utimer t_seq_encode("seq encoding", &seq_encoding_elapsed);
				// seq compress
				vector<bitset<8>> seq_encoded_bitset = seq_encode_bitset(chars, encoding_table);
			}
			cout << "seq encoding computed in: " << seq_encoding_elapsed << endl;
			total_elapsed += seq_encoding_elapsed;
		}

		// par_write_bits_chunks(compressed_chunks, "./outputs/output2");

		elapsed_times.push_back(total_elapsed);
	}

	append_to_csv(elapsed_times, "./outputs/seq2_results.csv");

	return 0;
}
