message(STATUS "Processing SpMV library...")
add_subdirectory(include)
add_subdirectory(src)

message(STATUS "Processing SpMV unit tests...")
add_subdirectory(tests)