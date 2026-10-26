include(spmv_add_test)

spmv_add_test(./example_unit_test.cpp)
spmv_add_test(./csr_basic_test.cpp) # class construction test
spmv_add_test(./csr_matvec_test.cpp)
spmv_add_test(./csr_accessor_test.cpp) # accessing accessor test 