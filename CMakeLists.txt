cmake_minimum_required(VERSION 3.16)

project(parallel_distrib_computing)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O3 -Wall -pthread")
set(EXEC output)

include_directories(includes)

file(GLOB SOURCES src/*.cpp)

add_executable(${EXEC} ${SOURCES})