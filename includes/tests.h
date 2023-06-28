#ifndef PARALLEL_DISTRIB_COMPUTING_TESTS_H
#define PARALLEL_DISTRIB_COMPUTING_TESTS_H

#include "../includes/utils.h"
#include "../includes/huffman.h"
#include "../includes/ff_solution.h"
#include "../includes/nt_solution.h"
#include "../includes/seq_solution.h"

#include <iostream>
#include <vector>
#include <bitset>
#include <random>
#include <ff/ff.hpp>
#include <ff/parallel_for.hpp>

#define ASSERT_TRUE_MSG(x, msg) { if (!(x)) std::cout << __FUNCTION__ << " failed on line " << __LINE__ << std::endl; else std::cout << msg << std::endl; }
#define ASSERT_TRUE(x) { if (!(x)) std::cout << __FUNCTION__ << " failed on line " << __LINE__ << std::endl; }

void test_parallel_read();

void test_parallel_bitset_write();

/// @brief fastflow solution tests
void test_fastflow_encoding(int input_size);

/// @brief native threads solution tests
void test_native_threads_encoding(int input_size);

// helpers
std::vector<char> generate_random_ascii(int length);

#endif // PARALLEL_DISTRIB_COMPUTING_TESTS_H
