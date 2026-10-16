cmake_minimum_required(VERSION 3.10)

project(SpMV VERSION 0.1.0)
enable_language(CXX)


#===============================================================================
# Basic CMake configuration
#===============================================================================

# Disable in-source builds
if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR
    "In-source builds are not allowed."
    " Create a separate directory for build files and delete CMakeCache.txt.")
endif()

# If no build type is specified, default to Debug 
if (NOT CMAKE_BUILD_TYPE)
  message(STATUS "No build type selected, defaulting to Debug")
  set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the C++ standard
set(SPMV_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD ${SPMV_CXX_STANDARD} CACHE STRING "Default C++ standard")
set(CMAKE_CXX_STANDARD_REQUIRED ON CACHE BOOL "Require C++ standard")

# Add our CMake directory to search path for CMake macros and functions
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Set warning flags
include(spmv_add_cxx_compiler_flag)
spmv_add_cxx_compiler_flag("-Wall")
spmv_add_cxx_compiler_flag("-Wextra")
spmv_add_cxx_compiler_flag("-Wcast-align")
spmv_add_cxx_compiler_flag("-Wcast-qual")
spmv_add_cxx_compiler_flag("-Wconversion")
spmv_add_cxx_compiler_flag("-Wdouble-promotion")
spmv_add_cxx_compiler_flag("-Wfloat-equal")
spmv_add_cxx_compiler_flag("-Wimplicit-fallthrough")
spmv_add_cxx_compiler_flag("-Wmissing-noreturn")
spmv_add_cxx_compiler_flag("-Wnon-virtual-dtor")
spmv_add_cxx_compiler_flag("-Wshadow")
spmv_add_cxx_compiler_flag("-Wunused")
spmv_add_cxx_compiler_flag("-Wvla")
spmv_add_cxx_compiler_flag("-Wzero-as-null-pointer-constant")
spmv_add_cxx_compiler_flag("-Wunreachable-code")
spmv_add_cxx_compiler_flag("-Wmissing-noreturn")
spmv_add_cxx_compiler_flag("-Wtautological-compare")
spmv_add_cxx_compiler_flag("-Wundef")
spmv_add_cxx_compiler_flag("-Werror")
spmv_add_cxx_compiler_flag("-Wold-style-cast")
spmv_add_cxx_compiler_flag("-pedantic")
spmv_add_cxx_compiler_flag("-Wsign-conversion")

# Process the library and examples
add_subdirectory(spmv)
add_subdirectory(examples)