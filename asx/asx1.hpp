#include <iostream>
#include <vector>
#include <thread>
#include <string>
#include "utils.hpp"

#ifndef ASX1_HPP
#define ASX1_HPP

namespace asx1 
{
  void rand_init(std::vector<double>* input);

  void print_vector(const std::string& vec_name, std::vector<double> input);

  template <typename T>
  void apply_func_to_vec(std::vector<T>* v, T (*f)(T), int vlen, int nw) 
  {
    using namespace std;
    
    // worker thread
    auto worker = [&](int index) {
      int slice_len = vlen / nw;
      int start_index = index * slice_len;
      int next_worker_start_index = (index + 1) * slice_len;
      bool is_last_worker = index == nw - 1;
      int end_index = is_last_worker ? vlen : next_worker_start_index;

      { // utimer t0("sequential work");

        // I am sure to not have race conditions, because
        // threads are accessing different vector slices
        for (int i = start_index; i < end_index; i++)
          (*v)[i] = f((*v)[i]);
      }

      return;
    };

    vector<thread*> tids;
    long usecs;

    { utimer t1("parallel work", &usecs);

      // thread spawning
      for (int i = 0; i < nw; i++)
        tids.push_back(new thread(worker, i));
      
      // await thread completion
      for (auto tid: tids) tid->join();
    }

    std::cout << "End (spent " << usecs << " usecs using " << nw << " threads)"  << endl;
    
    print_vector("result vector", *v);  
  }

  void execute(int vlen, int nw);
}

#endif
