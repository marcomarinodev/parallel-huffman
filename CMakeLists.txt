cmake_minimum_required(VERSION 3.16)

project(parallel_distrib_computing)


set(CMAKE_CXX_STANDARD 17)
set(EXEC output)
set(CMAKE_CXX_FLAGS "-I/home/mamarino/fastflow -O3 -Wall -pthread -pedantic")

include_directories(includes)

file(GLOB SOURCES src/*.cpp)

add_executable(${EXEC} ${SOURCES})