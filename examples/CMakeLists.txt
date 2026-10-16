message(STATUS "Processing SpMV example(s)...")

# Define the executable
add_executable(spmv_example SpMV_example.cpp )

# Declare what directories to include and libraries to link to
target_include_directories(spmv_example PUBLIC ${SPMV_INCLUDE_DIR})
target_link_directories(spmv_example PUBLIC ${SPMV_LIB_DIR})
target_link_libraries(spmv_example PUBLIC spmv)