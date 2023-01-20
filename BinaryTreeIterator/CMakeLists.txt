cmake_minimum_required(VERSION 3.22)
project(iterator)
project(lib)

set(CMAKE_CXX_STANDARD 14)

add_executable(iterator main.cpp)
add_library(iterator_lib BinaryTree.h BinaryTree.cpp)
add_subdirectory("Googletest")
