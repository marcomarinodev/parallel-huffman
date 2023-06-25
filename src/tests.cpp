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

void test_parallel_write()
{
}

void test_parallel_bitset_write()
{
  // clear test_outputs content
  std::ofstream ofs;
  ofs.open("./outputs/test_output1.bin", std::ofstream::out | std::ofstream::trunc);
  ofs.close();

  ofs.open("./outputs/test_output2.bin", std::ofstream::out | std::ofstream::trunc);
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

void test_fastflow_encoding()
{
  vector<char> chars = generate_random_ascii(1000);
  map<char, int> par_map_chars, seq_map_chars;

  // parallel counting
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

  // sequential counting
  seq_map_chars = count_chars(chars);

  ASSERT_TRUE(par_map_chars == seq_map_chars);

  // build encoding table
  min_heap_node *huffman_tree = build_huffman_tree(par_map_chars);
	unordered_map<char, string> encoding_table = build_encoding_table(huffman_tree);

  // parallel encoding
  string par_encoded_string = par_encode(chars, encoding_table);

  // sequential encoding
  string seq_encoded_string = seq_encode_string(chars, encoding_table);

  ASSERT_TRUE_MSG(par_encoded_string == seq_encoded_string, "test fastflow encoding - PASSED");
} 

void test_native_threads_encoding()
{
  vector<char> chars = generate_random_ascii(1000);
  map<char, int> par_map_chars, par_map_gmr_chars, seq_map_chars;
  int map_nw = thread::hardware_concurrency();
  int red_nw = map_nw / 2;
  Asx3 asx3(map_nw, red_nw);

  par_map_chars = par_count_chars(chars);
  par_map_gmr_chars = asx3.par_gmr<char, int, char>(chars, map_nw, red_nw);
  seq_map_chars = count_chars(chars);  

  ASSERT_TRUE(par_map_chars == seq_map_chars);
  ASSERT_TRUE(par_map_gmr_chars == seq_map_chars);

  // build encoding table
  min_heap_node *huffman_tree = build_huffman_tree(par_map_chars);
	unordered_map<char, string> encoding_table = build_encoding_table(huffman_tree);

  // parallel encoding
  string par_encoded_string = par_encode(chars, encoding_table);

  // sequential encoding
  string seq_encoded_string = seq_encode_string(chars, encoding_table);

  ASSERT_TRUE_MSG(par_encoded_string == seq_encoded_string, "test native threads encoding - PASSED");
}

// helpers
vector<char> generate_random_ascii(int length) {
  std::vector<char> result;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 128);
    for (int i = 0; i < length; i++) {
        result.push_back(static_cast<char>(dis(gen)));
    }
    return result;
}
