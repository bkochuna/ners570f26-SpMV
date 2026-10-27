#include "SpMV.hpp"
#include <vector>
#include <stdexcept>
#include <cmath>
#include "unit_test_framework.hpp"

// considering that the assembe method takes 3 arguments: nrows, ncols, max_nnz (not sure who wrote it)
// 1. FORMATTING TESTS
TEST_CASE(test_format_verify_output_geometry) {
    int rows = 4;
    int max_nnz = 3;
    SpMV::SparseMatrix_ELL ell(rows, 4, max_nnz);
    ell.assemble({0, 1}, {1, 2}, {5.0, 6.0});

    // Verify format of assemble output arrays: Must be 1D structures
    size_t expected_flat_size = static_cast<size_t>(rows * max_nnz);
    
    ASSERT(ell.cols.size() == expected_flat_size);
    ASSERT(ell.values.size() == expected_flat_size);
}

TEST_CASE(test_format_verify_input_mismatched_bounds) {
    SpMV::SparseMatrix_ELL ell(3, 3, 2);
    
    // Simulate passing invalid dimensions by generating coordinate arrays with mismatched lengths
    std::vector<int> rows = {0, 1, 2};
    std::vector<int> columns = {1, 2}; // Length mismatch error
    std::vector<double> data = {10.0, 20.0, 30.0};

    bool caught = false;
    try {
        ell.assemble(rows, columns, data);
    } catch (const std::invalid_argument&) {
        caught = true;
    }
    ASSERT(caught);
}

// Make sure col_idx has int entries
TEST_CASE(test_format_verify_column_integer_type) {
    bool is_int_type = std::is_same<int, typename decltype(SpMV::SparseMatrix_ELL::cols)::value_type>::value;
    ASSERT(is_int_type);
}


// 2. EDGE CASE TESTS
TEST_CASE(test_edge_empty_matrix) {
    SpMV::SparseMatrix_ELL ell(4, 4, 2);
    ell.assemble({}, {}, {});

    // Ensure structural allocation is cleanly padded with 0.0 and -1
    for (double val : ell.values) {
        ASSERT_NEAR(val, 0.0, 1e-14);
    }
    for (int col : ell.cols) {
        ASSERT(col == -1);
    }
}

// 3. COMPARISON TESTS
TEST_CASE(test_comparison_dense_equivalence) {
    SpMV::SparseMatrix_ELL ell(3, 3, 2);
    ell.assemble({0, 0, 1, 2}, {0, 2, 1, 0}, {10.0, 20.0, 30.0, 40.0});

    std::vector<double> expected_dense = {
        10.0,  0.0, 20.0,
         0.0, 30.0,  0.0,
        40.0,  0.0,  0.0
    };

    std::vector<double> actual_dense = ell.to_dense();
    ASSERT(actual_dense.size() == expected_dense.size());
    
    for (size_t i = 0; i < expected_dense.size(); ++i) {
        ASSERT_NEAR(actual_dense[i], expected_dense[i], 1e-14);
    }
}

// TEST SUITES ENCAPSULATION
TEST_SUITE(ell_matrix_all_tests) {
    TEST(test_format_verify_output_geometry);
    TEST(test_format_verify_input_mismatched_bounds);
    TEST(test_format_verify_column_integer_type);
    TEST(test_edge_empty_matrix);
    TEST(test_comparison_dense_equivalence);
}

auto main() -> int {
    RUN_SUITE(ell_matrix_all_tests);
    return 0;
}
