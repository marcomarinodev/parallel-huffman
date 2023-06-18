// #include <ff/parallel_for.hpp>
#include "../includes/tests.h"
#include "../includes/asx1.h"
#include "../includes/asx2.h"
#include "asx3.cpp"
#include "../includes/huffman.h"

#include <future>
#include <filesystem>
#include <list>
#include <bitset>
#include <condition_variable>
#include <unordered_map>

using namespace std;

int main(int argc, char *argv[])
{
	cout << "starting" << endl;
	// cli arguments
	const bool print_res = argc > 1 && !strcmp(argv[1], "p");
	const string filename = argc > 2 ? argv[2] : "sample";

	cout << "reading filename: " << filename << endl;

	vector<char> chars;

	{
		utimer seq_read("seq read");
		chars = read_file("./inputs/" + filename + ".txt");
	}

	map<char, int> par_map_chars, seq_map_chars;

	Asx3 asx3(2, 2);

	int map_nw = asx3.def_map_nw;
	int red_nw = asx3.def_red_nw;

	auto mapper = [](char w)
	{ return make_pair(w, 1); };
	auto reduce = [](int a, int b)
	{ return a + b; };

	{
		utimer t0("par_words_count");
		par_map_chars = asx3.par_gmr<char, int, char>(chars, map_nw, red_nw);
	}

	{
		utimer t1("seq_words_count");
		seq_map_chars = asx3.gmr<char, int, char>(chars, mapper, reduce);
	}

	if (print_res)
	{
		for (auto character : par_map_chars)
			cout << "(" << character.first << ", " << character.second << ")" << endl;
	}

	if (par_map_chars == seq_map_chars)
		cout << "The result is correct!" << endl;
	else
		cout << "Invalid result!" << endl;

	min_heap_node *huffman_tree = build_huffman_tree(par_map_chars);
	unordered_map<char, string> encoding_table = build_encoding_table(huffman_tree);

	for (const auto &[character, code] : encoding_table)
	{
		cout << character << ": " << code << endl;

		char code_char_arr[code.size()];
		strcpy(code_char_arr, code.c_str());
	}

	// vector<vector<bitset<8>>> compressed_chunks = par_compress(chars, encoding_table);

	// par_write_bits_chunks(compressed_chunks, "./outputs/output2");

	return 0;
}
