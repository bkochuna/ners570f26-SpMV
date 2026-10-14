hello(${CMAKE_CURRENT_SOURCE_DIR})

# Create a CMake variable listing all the source files
file(GLOB SPMV_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

# Define the software library spmv (libspmv.a)
add_library(spmv ${SPMV_SOURCES})

# Define include dirs for library
target_include_directories(spmv PUBLIC ${SPMV_INCLUDE_DIR})

#Save variable as a global variable (that's what CACHE means) for later use
set(SPMV_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE STRING "Include directory for linking spmv library")