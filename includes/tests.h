#ifndef PARALLEL_DISTRIB_COMPUTING_TESTS_H
#define PARALLEL_DISTRIB_COMPUTING_TESTS_H

#include "../includes/utils.h"
#include <iostream>
#include <vector>

#define ASSERT_TRUE(x) { if (!(x)) std::cout << __FUNCTION__ << " failed on line " << __LINE__ << std::endl; }

void test_parallel_read();
void test_parallel_write();

#endif // PARALLEL_DISTRIB_COMPUTING_TESTS_H
