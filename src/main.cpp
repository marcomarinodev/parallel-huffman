#include <cstring>
// #include <ff/parallel_for.hpp>
#include "../includes/asx1.h"
#include "../includes/asx2.h"
#include "asx3.cpp"
#include "../includes/huffman.h"
#include <cerrno>

using namespace std;

void concurrent_write(const string &str, int start, int end, const string &filename)
{
	ofstream file(filename, ios::in | ios::out);
	if (file.is_open())
	{
		file.seekp(start);
		for (int i = start; i < end; i++)
			file << str[i];
		file.close();
		return;
	}
	
	// handling opening file error
	cerr << "Error: failed to open file " << filename << " " << strerror(errno) << endl;
}

int main(int argc, char *argv[])
{
	cout << "starting" << endl;
	// cli arguments
	const bool print_res = argc > 1 && !strcmp(argv[1], "p");
	const string filename = argc > 2 ? argv[2] : "sample";

	cout << "reading filename: " << filename << endl;

	vector<char> chars = read_file("./inputs/" + filename + ".txt");
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
	map<char, string> encoding_table = build_encoding_table(huffman_tree);

	for (const auto &[character, code] : encoding_table)
	{
		cout << character << ": " << code << endl;

		char code_char_arr[code.size()];
		strcpy(code_char_arr, code.c_str());
	}

	// encoding phase
	const string str = "Hello World!";
	const int num_threads = 3;
	const int chunk_size = str.size() / num_threads;
	thread threads[num_threads];
	
	for (int i = 0; i < num_threads; i++)
	{
		int start = i * chunk_size;
		int end = (i == num_threads - 1) ? str.size() : start + chunk_size;

		threads[i] = thread(concurrent_write, str, start, end, "./outputs/output.txt");
	}

	for (int i = 0; i < num_threads; i++)
		threads[i].join();

	return 0;
}