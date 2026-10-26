#include "SpMV.hpp"
#include "SparseMatrix_ELL.hpp"
#include "unit_test_framework.hpp"
#include <vector> 
#include <stdexcept> 

using fp_type = double;

// Create a unit test
TEST_CASE(compare_get_col_idx) 
{
  SpMV::SparseMatrix_ELL<fp_type> ell(3,3);
  std::vector<size_t> const col_idx = {0,0,1,1,2,2,0,0,2};
  ell.set_col_idx(col_idx);
  std::vector<size_t> const& col_idx_ell = ell.get_col_idx();
  ASSERT(col_idx_ell == col_idx);
}

TEST_CASE(compare_get_values)
{
  SpMV::SparseMatrix_ELL<fp_type> ell(3,3);
  std::vector<fp_type> const values = {0.1,0.2,0.3,0.1,0.2,0.4,0.7,0.1,0.8};
  ell.set_values(values);
  std::vector<fp_type> const& values_ell = ell.get_values();
  ASSERT(values_ell.size() == values.size());
  for (size_t i = 0; i<values.size(); i++){
    ASSERT_NEAR(values_ell[i], values[i], 1e-3);
  }
}

TEST_CASE(compare_get_max_length_row)
{
  SpMV::SparseMatrix_ELL<fp_type> ell(3,3);
  size_t const max_length_row = 3;
  ell.set_max_length_row(max_length_row);
  size_t const max_length_row_ell = ell.get_max_length_row();
  ASSERT(max_length_row_ell == max_length_row); 
}

TEST_CASE(col_idx_override)
{
  SpMV::SparseMatrix_ELL<fp_type> ell(3,3);
  std::vector<size_t> const col_idx = {0,0,1,1,2,2,0,0,2};
  ell.set_col_idx(col_idx);
  ASSERT(ell.get_col_idx() == col_idx);
  std::vector<size_t> const col_idx2 = {0,1,2,2,1,0,1,1,1};
  ell.set_col_idx(col_idx2);
  ASSERT(ell.get_col_idx() == col_idx2);
}

TEST_CASE(values_override)
{
  SpMV::SparseMatrix_ELL<fp_type> ell(3,3);
  std::vector<fp_type> const values = {0.1,0.2,0.3,0.1,0.2,0.4,0.7,0.1,0.8};
  ell.set_values(values);
  std::vector<fp_type> const& values_ell = ell.get_values();
  ASSERT(values_ell.size() == values.size());
  for (size_t i = 0; i<values.size(); i++){
    ASSERT_NEAR(values_ell[i], values[i], 1e-3);
  }
  std::vector<fp_type> const values2 = {0.3,0.2,0.1,0.4,0.5,0.6,0.6,0.3,0.1};
  ell.set_values(values2);
  std::vector<fp_type> const& values_ell2 = ell.get_values();
  ASSERT(values_ell2.size() == values2.size());
  for (size_t i = 0; i<values2.size(); i++){
    ASSERT_NEAR(values_ell2[i], values2[i], 1e-3);
  }
}

TEST_CASE(max_length_row_override)
{
  SpMV::SparseMatrix_ELL<fp_type> ell(3,3);
  ell.set_max_length_row(3);
  ASSERT(ell.get_max_length_row() == 3);
  ell.set_max_length_row(2);
  ASSERT(ell.get_max_length_row() == 2); 
}

TEST_CASE(col_idx_edge_case) 
{
  SpMV::SparseMatrix_ELL<fp_type> ell(3,3);
  std::vector<size_t> const col_idx = {};
  ell.set_col_idx(col_idx);
  ASSERT(ell.get_col_idx().empty());
}

TEST_CASE(values_edge_case)
{
  SpMV::SparseMatrix_ELL<fp_type> ell(3,3);
  std::vector<fp_type> const values = {};
  ell.set_values(values);
  ASSERT(ell.get_values().empty());
}

TEST_CASE(max_length_edge_case1)
{
  SpMV::SparseMatrix_ELL<fp_type> ell(3,3);
  size_t const max_length_row_invalid = 10;
  size_t error = 0;
  
  try {ell.set_max_length_row(max_length_row_invalid);} catch (const std::invalid_argument& e){error = 1;}
  ASSERT(error == 1); 
}

TEST_CASE(max_length_edge_case2)
{
  SpMV::SparseMatrix_ELL<fp_type> ell(3,3);
  size_t const max_length_row_invalid = 'A';
  size_t error = 0;
  
  try {ell.set_max_length_row(max_length_row_invalid);}
  catch (const std::invalid_argument& e){error = 1;}
  ASSERT(error == 1); 
}


// Create a test suite
TEST_SUITE(test_getters_suite) 
{
  TEST(compare_get_col_idx);
  TEST(compare_get_values);
  TEST(compare_get_max_length_row);  
}

// Create a test suite
TEST_SUITE(override_suite) 
{
  TEST(col_idx_override);
  TEST(values_override);
  TEST(max_length_row_override);  
}

TEST_SUITE(edge_case) 
{
  TEST(col_idx_edge_case);
  TEST(values_edge_case);
  TEST(max_length_edge_case1); 
  TEST(max_length_edge_case2);  
}

auto
main() -> int
{
  RUN_SUITE(test_getters_suite);
  RUN_SUITE(override_suite);
  RUN_SUITE(edge_case);
  return 0; 
}
