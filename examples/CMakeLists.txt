hello(${CMAKE_CURRENT_SOURCE_DIR})

# Define the executable
add_executable(spmv_example SpMV_example.cpp )

MESSAGE(STATUS "SPMV_INCLUDE_DIR=${SPMV_INCLUDE_DIR}")

# Declare what directories to include
target_include_directories(spmv_example PUBLIC ${SPMV_INCLUDE_DIR})