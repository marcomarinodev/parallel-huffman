#include "asx1.hpp"

using namespace std;

void asx1::rand_init(vector<double>* input) 
{
  int seed = 123;
  const int max = 10;
  srand(seed);

  for (size_t i = 0; i < (*input).size(); i++)
    (*input)[i] = (double) (rand() % max);
}

void asx1::print_vector(string vec_name, vector<double> input) 
{
  cout << vec_name << ": "; 
  for (size_t i = 0; i < input.size(); i++)
    cout << input[i] << " ";
  cout << endl;
}
