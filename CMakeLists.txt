cmake_minimum_required(VERSION 3.10)

project(SpMV VERSION 0.1.0)

enable_language(CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(hello)

add_subdirectory(spmv)
add_subdirectory(examples)