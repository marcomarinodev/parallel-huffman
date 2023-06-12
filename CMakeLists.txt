cmake_minimum_required(VERSION 3.16)

project(parallel_distrib_computing)


set(CMAKE_CXX_STANDARD 17)
set(EXEC output)
set(FF "/mnt/c/Users/marco.marino/fastflow")
set(CMAKE_CXX_FLAGS "-O3 -Wall -pthread -pedantic")

include_directories(includes)
include_directories(${FF})

file(GLOB SOURCES src/*.cpp)

add_executable(${EXEC} ${SOURCES})