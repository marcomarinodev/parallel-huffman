#include "../includes/tests.h"

using namespace std;

void test_parallel_read()
{
  vector<char> seq_chars;
  vector<char> par_chars;

  // 10k.txt
  seq_chars = read_file("./inputs/10k.txt");
  par_chars = par_read_file("./inputs/10k.txt", 2);
  ASSERT_TRUE(seq_chars == par_chars);

  // 100k.txt
  seq_chars = read_file("./inputs/100k.txt");
  par_chars = par_read_file("./inputs/100k.txt", 2);
  ASSERT_TRUE(seq_chars == par_chars);

  // 1000.txt
  seq_chars = read_file("./inputs/1000.txt");
  par_chars = par_read_file("./inputs/1000.txt", 2);
  ASSERT_TRUE(seq_chars == par_chars);

  // sample.txt
  seq_chars = read_file("./inputs/sample.txt");
  par_chars = par_read_file("./inputs/sample.txt", 2);
  ASSERT_TRUE(seq_chars == par_chars);

  cout << "TEST PASSED!" << endl;
}

void test_parallel_bitset_write()
{
  // clear test_outputs content
  ofstream ofs;
  ofs.open("./outputs/test_output1.bin", ofstream::out | ofstream::trunc);
  ofs.close();

  ofs.open("./outputs/test_output2.bin", ofstream::out | ofstream::trunc);
  ofs.close();

  // Test case 1: Write a single chunk of data
  vector<vector<bitset<8>>> chunks1 = {{bitset<8>("01010101"), bitset<8>("10101010")}};
  string hex_chunks1 = "\x55\xAA";
  par_write_bits_chunks(chunks1, "./outputs/test_output1.bin");

  ifstream in_file1("./outputs/test_output1.bin", ios::binary);
  ASSERT_TRUE(in_file1);

  string contents1((istreambuf_iterator<char>(in_file1)), istreambuf_iterator<char>());
  ASSERT_TRUE(contents1 == hex_chunks1);
  in_file1.close();

  // Test case 2: Write multiple chunks of data
  vector<vector<bitset<8>>> chunks2 = {{bitset<8>("11110000"), bitset<8>("00001111")}, {bitset<8>("00110011"), bitset<8>("11001100")}};
  string hex_chunks2 = "\xF0\x0F\x33\xCC";
  par_write_bits_chunks(chunks2, "./outputs/test_output2.bin");

  ifstream in_file2("./outputs/test_output2.bin", ios::binary);
  ASSERT_TRUE(in_file2);
  
  string contents2((istreambuf_iterator<char>(in_file2)), istreambuf_iterator<char>());
  ASSERT_TRUE(contents2 == hex_chunks2);
  in_file2.close();
}

void test_fastflow_encoding(int input_size)
{
  cout << "\n==========" << endl;
  cout << "[TEST FASTFLOW ENCODING] - Input Size: " << input_size << endl;
  vector<char> chars = generate_random_ascii(input_size);
  
  // fastflow counting
  map<char, int> par_map_chars = ff_solution::count_chars(chars);

  // sequential counting
  map<char, int> seq_map_chars = seq_solution::count_chars(chars);

  ASSERT_TRUE(par_map_chars == seq_map_chars);

  // build encoding table
  min_heap_node *huffman_tree = build_huffman_tree(par_map_chars);
	unordered_map<char, string> encoding_table = build_encoding_table(huffman_tree);

  // fastflow encoding
  string par_encoded_string = ff_solution::encode(chars, encoding_table);
  
  // sequential encoding
  string seq_encoded_string = seq_solution::encode(chars, encoding_table);

  ASSERT_TRUE_MSG(par_encoded_string == seq_encoded_string, "[TEST FASTFLOW ENCODING] - PASSED");
} 

void test_native_threads_encoding(int input_size)
{
  cout << "\n==========" << endl;
  cout << "[TEST NATIVE THREADS ENCODING] - Input Size: " << input_size << endl;
  vector<char> chars = generate_random_ascii(input_size);
  map<char, int> par_map_chars, par_map_gmr_chars, seq_map_chars;

  int map_nw = thread::hardware_concurrency();
  int red_nw = map_nw / 2;

  par_map_chars = nt_solution::count_chars(chars);
  par_map_gmr_chars = nt_solution::Gmr(map_nw, red_nw).count_chars(chars);
  seq_map_chars = seq_solution::count_chars(chars);  

  ASSERT_TRUE(par_map_chars == seq_map_chars && par_map_gmr_chars == seq_map_chars);

  // build encoding table
  min_heap_node *huffman_tree = build_huffman_tree(par_map_chars);
	unordered_map<char, string> encoding_table = build_encoding_table(huffman_tree);

  // parallel encoding
  string par_encoded_string = nt_solution::encode(chars, encoding_table);

  // sequential encoding
  string seq_encoded_string = seq_solution::encode(chars, encoding_table);

  ASSERT_TRUE_MSG(par_encoded_string == seq_encoded_string, "[TEST NATIVE THREADS ENCODING] - PASSED");
}

// helpers
vector<char> generate_random_ascii(int length) {
  cout << "Generating Random ASCII" << endl;
  vector<char> result;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 128);
    for (int i = 0; i < length; i++) {
        result.push_back(static_cast<char>(dis(gen)));
    }
    return result;
}
