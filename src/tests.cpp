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
{}
