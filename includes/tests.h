#ifndef PARALLEL_DISTRIB_COMPUTING_TESTS_H
#define PARALLEL_DISTRIB_COMPUTING_TESTS_H

#include "../includes/utils.h"
#include "../includes/huffman.h"
#include "../src/asx3.cpp"
#include <iostream>
#include <vector>
#include <bitset>
#include <random>
#include <ff/ff.hpp>
#include <ff/parallel_for.hpp>

#define ASSERT_TRUE_MSG(x, msg) { if (!(x)) std::cout << __FUNCTION__ << " failed on line " << __LINE__ << std::endl; else std::cout << msg << std::endl; }

#define ASSERT_TRUE(x) { if (!(x)) std::cout << __FUNCTION__ << " failed on line " << __LINE__ << std::endl; }

void test_parallel_read();

void test_parallel_write();

void test_parallel_bitset_write();

void test_fastflow_encoding();

void test_native_threads_encoding();

// helpers
std::vector<char> generate_random_ascii(int length);

#endif // PARALLEL_DISTRIB_COMPUTING_TESTS_H
