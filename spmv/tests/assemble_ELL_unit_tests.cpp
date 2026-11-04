#include "SparseMatrix_ELL.hpp"
#include <vector>
#include <stdexcept>
#include <type_traits>
#include "unit_test_framework.hpp"

// 1. FORMATTING TESTS
TEST_CASE(test_format_verify_output_geometry) {
    size_t rows = 4;
    size_t cols = 4;
    
    SpMV::SparseMatrix_ELL<double> ell(rows, cols);
    ell.setValue(0, 1, 5.0);
    ell.setValue(1, 2, 6.0);
    ell.assemble();

    ASSERT(ell.getFormat() == SpMV::MatrixType::ELL);
}

TEST_CASE(test_format_verify_input_mismatched_bounds) {
    SpMV::SparseMatrix_ELL<double> ell(3, 3);
    
    // Testing out-of-bounds index insertion behavior
    bool caught = false;
    try {
        ell.setValue(10, 2, 30.0); // Out of row bounds
    } catch (const std::out_of_range&) {
        caught = true;
    } catch (const std::invalid_argument&) {
        caught = true;
    }
    ASSERT(caught);
}

// 2. EDGE CASE TESTS
TEST_CASE(test_edge_empty_matrix) {
    SpMV::SparseMatrix_ELL<double> ell(4, 4);
    ell.assemble();

    // Check retrieving unassigned values returns 0.0
    ASSERT_NEAR(ell.getValue(0, 0), 0.0, 1e-14);
    ASSERT_NEAR(ell.getValue(2, 3), 0.0, 1e-14);
}

// 3. COMPARISON TESTS
TEST_CASE(test_comparison_dense_equivalence) {
    SpMV::SparseMatrix_ELL<double> ell(3, 3);
    ell.setValue(0, 0, 10.0);
    ell.setValue(0, 2, 20.0);
    ell.setValue(1, 1, 30.0);
    ell.setValue(2, 0, 40.0);
    ell.assemble();

    ASSERT_NEAR(ell.getValue(0, 0), 10.0, 1e-14);
    ASSERT_NEAR(ell.getValue(0, 1), 0.0, 1e-14);
    ASSERT_NEAR(ell.getValue(0, 2), 20.0, 1e-14);
    ASSERT_NEAR(ell.getValue(1, 1), 30.0, 1e-14);
    ASSERT_NEAR(ell.getValue(2, 0), 40.0, 1e-14);
}

// TEST SUITES ENCAPSULATION
TEST_SUITE(ell_matrix_all_tests) {
    TEST(test_format_verify_output_geometry);
    TEST(test_format_verify_input_mismatched_bounds);
    TEST(test_edge_empty_matrix);
    TEST(test_comparison_dense_equivalence);
}

auto main() -> int {
    RUN_SUITE(ell_matrix_all_tests);
    return 0;
}
