#include <iostream>
#include "./asx/asx1.hpp"
#include <omp.h>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) 
{
  int vlen = argc > 1 ? atoi(argv[1]) : 10;
  int nw = argc > 2 ? atoi(argv[2]) : 4;
  
  vector<double> v(vlen);

  asx1::rand_init(&v);
  asx1::print_vector("Initial input vector", v);

  auto f = [] (double x) { return x * 2; };

  asx1::apply_func_to_vec<double>(&v, f, vlen, nw);

  return 0;
}