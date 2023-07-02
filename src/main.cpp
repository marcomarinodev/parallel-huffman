#include "../includes/tests.h"
#include "../includes/huffman.h"
#include "../includes/seq_solution.h"
#include "../includes/ff_solution.h"
#include "../includes/nt_solution.h"

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

enum mode
{
	seq = 0,
	nt_par = 1,
	ff_par = 2
};

int main(int argc, char *argv[])
{

	bool perform_tests = argc > 1 ? strcmp(argv[1], "test") == 0 : false;
	int test_input_size = argc > 2 ? atoi(argv[2]) : 1000;
	int mode = argc > 3 ? atoi(argv[3]) : ff_par;
	bool performance_test = argc > 4 ? strcmp(argv[4], "ptest") == 0 : false;

	vector<string> input_filenames;
	vector<string> output_filenames;
	vector<int> nums_threads;
	vector<long> elapsed_stage_times;
	vector<long> par_computing_time_avgs;
	vector<long> execution_times;

	cout << "=> Performing tests <=" << endl;

	if (perform_tests)
	{
		test_fastflow_encoding(test_input_size);
		test_native_threads_encoding(test_input_size);
		return 0;
	}

	input_filenames.push_back("./inputs/10M");
	input_filenames.push_back("./inputs/50M");
	input_filenames.push_back("./inputs/100M");

	output_filenames.push_back("./outputs/nt_par_s.csv");
	output_filenames.push_back("./outputs/nt_par_m.csv");
	output_filenames.push_back("./outputs/nt_par_l.csv");

	nums_threads.push_back(1);
	nums_threads.push_back(2);
	nums_threads.push_back(4);
	nums_threads.push_back(8);
	nums_threads.push_back(16);
	nums_threads.push_back(32);

	for (int i = 0; i < input_filenames.size(); i++)
	{
		string input_filename = input_filenames[i];
		cout << "\n========================" << endl;
		cout << "analyzing: " << input_filename << endl;
		vector<char> chars;

		// FILE READING
		chars = read_file(input_filename);

		bool do_parallel_policy = mode != seq;

		for (auto num_thread : nums_threads)
		{
			long par_compute_time_avg = 0;

			for (int j = 0; j < 10; j++)
			{
				// => COUNTING CHARS <=
				map<char, int> par_map_chars, seq_map_chars;

				if (do_parallel_policy)
				{
					cout << "file big => parallel counting" << endl;
					long par_word_count_elapsed;
					{
						utimer par_words_count("par words count", &par_word_count_elapsed);

						if (mode == nt_par)
						// google map reduce
							// par_map_chars = nt_solution::Gmr(num_thread, num_thread / 2 + 1).count_chars(chars);
							par_map_chars = nt_solution::count_chars(chars, num_thread);
						else
							par_map_chars = ff_solution::count_chars(chars);
					}
					cout << "par words count computed in: " << par_word_count_elapsed << endl;
					elapsed_stage_times.push_back(par_word_count_elapsed);
					par_compute_time_avg += par_word_count_elapsed;
				}
				else
				{
					cout << "file small => sequential counting" << endl;
					long seq_words_count_elapsed;
					{
						utimer seq_words_count("seq words count", &seq_words_count_elapsed);
						seq_map_chars = seq_solution::count_chars(chars);
					}
					cout << "seq words count computed in: " << seq_words_count_elapsed << endl;
					elapsed_stage_times.push_back(seq_words_count_elapsed);
				}

				// ENCODING TABLE BUILDER
				min_heap_node *huffman_tree;
				unordered_map<char, string> encoding_table;
				long build_encoding_table_elasped;
				{
					utimer t_build_encoding_table("build encoding table", &build_encoding_table_elasped);
					huffman_tree = build_huffman_tree(do_parallel_policy ? par_map_chars : seq_map_chars);
					encoding_table = build_encoding_table(huffman_tree);
				}
				cout << "build encoding table computed in: " << build_encoding_table_elasped << endl;
				elapsed_stage_times.push_back(build_encoding_table_elasped);
				par_compute_time_avg += build_encoding_table_elasped;

				// => ENCODING <=
				if (do_parallel_policy)
				{
					cout << "file big => parallel encoding" << endl;
					// [PAR]: parallel encoding
					long par_encoding_elapsed;
					{
						utimer t_par_encode("par encoding", &par_encoding_elapsed);
						string par_encoded_string;
						if (mode == nt_par)
							par_encoded_string = nt_solution::encode(chars, encoding_table, num_thread);
						else
							par_encoded_string = ff_solution::encode(chars, encoding_table, num_thread);

						vector<bitset<8>> par_encoded_bitset = seq_solution::compress(par_encoded_string);
					}
					cout << "[PAR] {ENCODING + COMPRESSION}: " << par_encoding_elapsed << endl;
					elapsed_stage_times.push_back(par_encoding_elapsed);
					par_compute_time_avg += par_encoding_elapsed;
				}
				else
				{
					cout << "file small => sequential encoding" << endl;
					// [SEQ]: sequential encoding
					long seq_encoding_elapsed;
					{
						utimer t_seq_encode("seq encoding", &seq_encoding_elapsed);
						vector<bitset<8>> seq_encoded_bitset = seq_solution::encode_and_compress(chars, encoding_table);
					}
					cout << "[SEQ] {ENCODING + COMRPESSION}" << seq_encoding_elapsed << endl;
					elapsed_stage_times.push_back(seq_encoding_elapsed);
				}

				// append_to_csv(elapsed_stage_times, filename_to_append);

				elapsed_stage_times.clear();
			}

			par_compute_time_avg /= 10;
			par_computing_time_avgs.push_back(par_compute_time_avg);
			
		}

		string filename_to_append = output_filenames[i];
		append_to_csv(par_computing_time_avgs, filename_to_append);
	}

	return 0;
}
