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
	int mode = argc > 2 ? atoi(argv[2]) : ff_par;
	int num_thread = argc > 3 ? atoi(argv[3]) : 1;
	string filename = argc > 4 ? argv[4] : "./inputs/10M";
	bool do_parallel_policy = mode != seq;

	if (perform_tests)
	{
		cout << "=> Performing tests <=" << endl;
		test_fastflow_encoding(1000);
		test_native_threads_encoding(1000);
		return 0;
	}

	cout << "analyzing: " << filename << endl;
	vector<char> chars;
	chars = read_file(filename);

	int execution_time_avg;

	for (int j = 0; j < 5; j++)
	{
		long execution_time;
		{
			utimer execution_timer("exec time", &execution_time);
			// ----------------------------
			// ------ Conting chars -------
			// ----------------------------
			map<char, int> par_map_chars, seq_map_chars;

			if (do_parallel_policy)
			{
				cout << "file big => parallel counting" << endl;
				if (mode == nt_par)
					par_map_chars = nt_solution::Gmr(num_thread, num_thread / 2 + 1).count_chars(chars);
				else
					par_map_chars = ff_solution::count_chars(chars, num_thread);
			}
			else
			{
				cout << "file small => sequential counting" << endl;
				seq_map_chars = seq_solution::count_chars(chars);
			}

			// ----------------------------------
			// ------ Build Encoding Table ------
			// ----------------------------------
			min_heap_node *huffman_tree = build_huffman_tree(do_parallel_policy ? par_map_chars : seq_map_chars);
			;
			unordered_map<char, string> encoding_table = build_encoding_table(huffman_tree);

			// -----------------------------
			// ------ Encoding phase -------
			// -----------------------------
			if (do_parallel_policy)
			{
				cout << "file big => parallel encoding" << endl;
				string par_encoded_string;
				if (mode == nt_par)
					par_encoded_string = nt_solution::encode(chars, encoding_table, num_thread);
				else
					par_encoded_string = ff_solution::encode(chars, encoding_table, num_thread);
			}
			else
			{
				cout << "file small => sequential encoding" << endl;
				string seq_encoded_string = seq_solution::encode(chars, encoding_table);
			}

			// -----------------------------
			// --------- Compress ----------
			// -----------------------------
			// vector<bitset<8>> par_encoded_bitset = seq_solution::compress(par_encoded_string);
		}

		cout << "execution time: " << execution_time << endl;

		execution_time_avg += execution_time;
	}

	execution_time_avg /= 5;

	cout << endl << filename << ": " << execution_time_avg << endl;

	return 0;
}
