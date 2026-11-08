#include "SpMV.hpp"
#include "SparseMatrix.hpp"
#include "SparseMatrix_ELL.hpp"
#include <stdexcept> 
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

// Setters and Getters unit test 
//------------------------------------------------------------------------------------------------------------------
// test case for col_idx setter
template <typename T> TEST_CASE(compare_get_col_idx) 
{
  SpMV::SparseMatrix_ELL<T> ell(3,3);
  std::vector<size_t> const col_idx = {0,0,1,1,2,2,0,0,2};
  ell.set_col_idx(col_idx);
  std::vector<size_t> const& col_idx_ell = ell.get_col_idx();
  ASSERT(col_idx_ell == col_idx);
}

// test case for get_values setter
template <typename T> TEST_CASE(compare_get_values)
{
  SpMV::SparseMatrix_ELL<T> ell(3,3);
  std::vector<T> const values = {0.1,0.2,0.3,0.1,0.2,0.4,0.7,0.1,0.8};
  ell.set_values(values);
  std::vector<T> const& values_ell = ell.get_values();
  ASSERT(values_ell.size() == values.size());
  for (size_t i = 0; i<values.size(); i++){
    ASSERT_NEAR(values_ell[i], values[i], 1e-3);
  }
}

// test case for max_length_row setter
template <typename T> TEST_CASE(compare_get_max_length_row)
{
  SpMV::SparseMatrix_ELL<T> ell(3,3);
  size_t const max_length_row = 3;
  ell.set_max_length_row(max_length_row);
  size_t const max_length_row_ell = ell.get_max_length_row();
  ASSERT(max_length_row_ell == max_length_row); 
}

//override test case for col_idx
template <typename T> TEST_CASE(col_idx_override)
{
  SpMV::SparseMatrix_ELL<T> ell(3,3);
  std::vector<size_t> const col_idx = {0,0,1,1,2,2,0,0,2};
  ell.set_col_idx(col_idx);
  ASSERT(ell.get_col_idx() == col_idx);
  std::vector<size_t> const col_idx2 = {0,1,2,2,1,0,1,1,1};
  ell.set_col_idx(col_idx2);
  ASSERT(ell.get_col_idx() == col_idx2);
}

// override test case for values
template <typename T> TEST_CASE(values_override)
{
  SpMV::SparseMatrix_ELL<T> ell(3,3);
  std::vector<T> const values = {0.1,0.2,0.3,0.1,0.2,0.4,0.7,0.1,0.8};
  ell.set_values(values);
  std::vector<T> const& values_ell = ell.get_values();
  ASSERT(values_ell.size() == values.size());
  for (size_t i = 0; i<values.size(); i++){
    ASSERT_NEAR(values_ell[i], values[i], 1e-3);
  }
  std::vector<T> const values2 = {0.3,0.2,0.1,0.4,0.5,0.6,0.6,0.3,0.1};
  ell.set_values(values2);
  std::vector<T> const& values_ell2 = ell.get_values();
  ASSERT(values_ell2.size() == values2.size());
  for (size_t i = 0; i<values2.size(); i++){
    ASSERT_NEAR(values_ell2[i], values2[i], 1e-3);
  }
}

// override test case for mmax_length_row
template <typename T> TEST_CASE(max_length_row_override)
{
  SpMV::SparseMatrix_ELL<T> ell(3,3);
  ell.set_max_length_row(3);
  ASSERT(ell.get_max_length_row() == 3);
  ell.set_max_length_row(2);
  ASSERT(ell.get_max_length_row() == 2); 
}

// edge case when col_idx is empty
template <typename T> TEST_CASE(col_idx_edge_case) 
{
  SpMV::SparseMatrix_ELL<T> ell(3,3);
  std::vector<size_t> const col_idx = {};
  ell.set_col_idx(col_idx);
  ASSERT(ell.get_col_idx().empty());
}

// edge case when when values is empty
template <typename T> TEST_CASE(values_edge_case)
{
  SpMV::SparseMatrix_ELL<T> ell(3,3);
  std::vector<T> const values = {};
  ell.set_values(values);
  ASSERT(ell.get_values().empty());
}

// edge case when max_length_row is higher than the number of rows of the matrix
template <typename T> TEST_CASE(max_length_edge_case1)
{
  SpMV::SparseMatrix_ELL<T> ell(3,3);
  size_t const max_length_row_invalid = 10;
  size_t error = 0;
  
  try {ell.set_max_length_row(max_length_row_invalid);} catch (const std::invalid_argument& e){error = 1;}
  ASSERT(error == 1); 
}

// edge case when max_length_row is a character other than an integer
template <typename T> TEST_CASE(max_length_edge_case2)
{
  SpMV::SparseMatrix_ELL<T> ell(3,3);
  size_t const max_length_row_invalid = 'A';
  size_t error = 0;
  
  try {ell.set_max_length_row(max_length_row_invalid);}
  catch (const std::invalid_argument& e){error = 1;}
  ASSERT(error == 1); 
}

// test suite for the setters
template <typename T> TEST_SUITE(test_getters_suite) 
{
  TEST(compare_get_col_idx<T>);
  TEST(compare_get_values<T>);
  TEST(compare_get_max_length_row<T>);  
}

// test suite for the override
template <typename T> TEST_SUITE(override_suite) 
{
  TEST(col_idx_override<T>);
  TEST(values_override<T>);
  TEST(max_length_row_override<T>);  
}

//test suite for edge cases
template <typename T> TEST_SUITE(edge_case) 
{
  TEST(col_idx_edge_case<T>);
  TEST(values_edge_case<T>);
  TEST(max_length_edge_case1<T>); 
  TEST(max_length_edge_case2<T>);  
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
    // Run all unit tests for setters and getters
    RUN_SUITE(test_getters_suite<float>);
    RUN_SUITE(override_suite<float>);
    RUN_SUITE(edge_case<float>); 
    return 0;
}

