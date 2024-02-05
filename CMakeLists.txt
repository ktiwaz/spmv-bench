cmake_minimum_required(VERSION 3.10)
project(spmm-bench)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-g)

include_directories(lib)

add_subdirectory(lib)
add_subdirectory(bin)
add_subdirectory(test)

