#include "SpMV.hpp"
#include "SparseMatrix_CSR.hpp"
#include "unit_test_framework.hpp"


TEST_CASE(test_initialized_matrix_state) {
    SpMV::SparseMatrix_CSR<double> A(3, 3);
    ASSERT(A.getState() == SpMV::MatrixState::initialized);
}

TEST_CASE(test_num_rows) {
    SpMV::SparseMatrix_CSR<double> A(3, 5);
    ASSERT(A.getNrows() == 3);
}


TEST_CASE(test_num_cols) {
    SpMV::SparseMatrix_CSR<double> A(3, 5);
    ASSERT(A.getNcols() == 5);
}

TEST_CASE(test_nnz) {
    SpMV::SparseMatrix_CSR<double> A(3, 3);
    ASSERT(A.getNumnz() == 0);
}


TEST_SUITE(csr_tests) {
    TEST(test_initialized_matrix_state);
    TEST(test_num_rows);
    TEST(test_num_cols);
    TEST(test_nnz);
}

int main() {
    RUN_SUITE(csr_tests);
    return 0;
}