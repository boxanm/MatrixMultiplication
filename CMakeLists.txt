cmake_minimum_required(VERSION 3.12)
project(PJCSemestralWork)

set(CMAKE_CXX_STANDARD 14)

add_executable(PJCSemestralWork main.cpp matrix.hpp matrix.cpp matrix1D.hpp matrix1D.cpp)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(PJCSemestralWork Threads::Threads)