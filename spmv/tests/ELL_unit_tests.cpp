#include "SpMV.hpp"
#include "SparseMatrix.hpp"
#include "SparseMatrix_ELL.hpp"

#include <cstddef>
#include <vector>

// Testing library that's required for testing
#include "unit_test_framework.hpp"


// ============================================================================
// ==== BASIC TESTS: Implemented by leahghartman
// ============================================================================
// Test 1: Test initialization, including edge cases
template <typename T> TEST_CASE(test_initialization) {
    // Perform a standard initialization
    SpMV::SparseMatrix_ELL<T> test(3, 6);

    // Edge cases: no rows, no columns and the smallest non-empty matrix
    SpMV::SparseMatrix_ELL<T> test_0(0, 0);
    SpMV::SparseMatrix_ELL<T> test_1(1, 1);
}

// Test 2: Test that the format is reported as ELL
template <typename T> TEST_CASE(test_format) {
    // Initialize a matrix object and then assert that it is of the ELL type
    SpMV::SparseMatrix_ELL<T> test(2, 2);
    ASSERT(test.getFormat() == SpMV::MatrixType::ELL);
    ASSERT(test.printFormat() == "ELL");
}

// Test 3: Test that the constructor properly stores the dimensions, including
// for a non-square matrix where swapping rows and columns would be caught.
template <typename T> TEST_CASE(test_dim) {
    SpMV::SparseMatrix_ELL<T> test(6, 9);
    ASSERT(test.get_nrows() == 6);
    ASSERT(test.get_ncols() == 9);

    SpMV::SparseMatrix_ELL<T> test_0(0, 0);
    ASSERT(test_0.get_nrows() == 0);
    ASSERT(test_0.get_ncols() == 0);
}

// Test 4: Test that a newly constructed matrix holds no ELL data, since 
// nothing has been assembled yet.
template <typename T> TEST_CASE(test_initial_storage_empty) {
    SpMV::SparseMatrix_ELL<T> test(4, 4);
    ASSERT(test.get_max_length_row() == 0);
    ASSERT(test.get_col_idx().empty());
    ASSERT(test.get_values().empty());
}

// Basic test suite
template <typename T> TEST_SUITE(basic_suite) {
    TEST(test_initialization<T>);
    TEST(test_format<T>);
    TEST(test_dim<T>);
    TEST(test_initial_storage_empty<T>);
}

// NOTE: we can add everyone else's tests above this main definition, then call
// each suite inside the below main function.
//
// ============================================================================
// ==== MAIN: Implemented by leahghartman
// ============================================================================
auto main() -> int {
    // Run all of the unit tests for each type
    RUN_SUITE(basic_suite<float>);
    RUN_SUITE(basic_suite<double>);
    return 0;
}

