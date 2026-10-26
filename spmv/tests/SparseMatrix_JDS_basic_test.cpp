#include "SpMV.hpp"
#include "SparseMatrix.hpp"
#include "SparseMatrix_JDS.hpp"
//#include <gtest/gtest.h> commented because not sure if independent set/get tests are needed and do not totally understand how to do this

// Testing library required for testing (Always include this last!)
#include "unit_test_framework.hpp"

// Use ASSERT(condition) to test if a condition is true.
// Direct comparison of floating point numbers is not recommended, so we define
// ASSERT_NEAR(a, b, epsilon) to test if a and b are within epsilon of each
// other.

// class JDSSparseMatrixTestFixture : public ::testing::Test {  commented because not sure if independent set/get tests are needed and do not totally understand how to do this
// protected:
//     // Helper to test GET: Injects a hardcoded valid JDS structure
//     void injectRawJDSState(JDSSparseMatrix& matrix,
//                            const std::vector<double>& d,
//                            const std::vector<size_t>& c_idx,
//                            const std::vector<size_t>& jdl_arr,
//                            const std::vector<size_t>& perm) {
//         matrix.data = d;
//         matrix.col_idx = c_idx;
//         matrix.jdl = jdl_arr;
//         matrix.permutation = perm;
//     }
// };

// Test initialization
template <typename T> TEST_CASE(test_initialization) {
  // Standard initializations
  SpMV::SparseMatrix_JDS<T> test(3, 6);

  // Edge Cases
  SpMV::SparseMatrix_JDS<T> test_0(0, 0);
  SpMV::SparseMatrix_JDS<T> test_1(1, 1);
}

// Test Formats
template <typename T> TEST_CASE(test_get_format) {
  SpMV::SparseMatrix_JDS<T> test(2, 2);
  ASSERT(test.getFormat() == 0);
  ASSERT(test.printFormat() == "JDS");
}

// Test State
template <typename T> TEST_CASE(test_state) {
  SpMV::SparseMatrix_JDS<T> test(4, 4);

  ASSERT(test.getState() == SpMV::MatrixState::initialized);
  ASSERT(test.getCoeffMap().empty());
}

// Test Dimensions
template <typename T> TEST_CASE(test_dim) {
  SpMV::SparseMatrix_JDS<T> test(6, 9);
  auto nrows = test.getNrows();
  auto ncols = test.getNcols();
  auto nnz = test.getNumnz();

  ASSERT(nrows == 6);
  ASSERT(ncols == 9);
  ASSERT(nnz == 0);
}

// Basic Test Suite
template <typename T> TEST_SUITE(basic_suite) {
  TEST(test_initialization<T>);
  TEST(test_get_format<T>);
  TEST(test_state<T>);
  TEST(test_dim<T>);
}

// Accessor Test cases
template <typename T> TEST_CASE(test_get_initial_value) {
  SpMV::SparseMatrix_JDS<T> test(2, 2);
  auto val = test.getValue(0,0);
  ASSERT_NEAR(val,0,1e-3);
}

template <typename T> TEST_CASE(test_set_value) {
  SpMV::SparseMatrix_JDS<T> test(2, 2);
  test.setValue(0,0,1.0);
  val = test.getValue(0,0);
  ASSERT_NEAR(val, 1, 1e-3);
}

template <typename T> TEST_CASE(test_reset_value) {
  SpMV::SparseMatrix_JDS<T> test(2, 2);
  test.setValue(0,0,1.0);
  test.setValue(0,0,2.0);
  val = test.getValue(0,0);
  ASSERT_NEAR(val,0,2.0);
}

template <typename T> TEST_CASE(test_set_value_outofboundexception) {
  SpMV::SparseMatrix_JDS<T> test(2, 2);
  bool exceptionThrown = false;
  try
  {
      test.setValue(3,3,1.0);
  }
  catch(ExceptionType&) // special exception type
  {
      exceptionThrown = true;
  }
  catch(...) // or any exception at all
  {
      exceptionThrown = true;
  }
  ASSERT(exceptionThrown); 
}

template <typename T> TEST_CASE(test_get_value_outofboundexception) {
  SpMV::SparseMatrix_JDS<T> test(2, 2);
  bool exceptionThrown = false;
  try
  {
      test.getValue(3,3);
  }
  catch(ExceptionType&) // special exception type
  {
      exceptionThrown = true;
  }
  catch(...) // or any exception at all
  {
      exceptionThrown = true;
  }
  ASSERT(exceptionThrown); 
}

template <typename T> TEST_SUITE(accessor_suite) {
  TEST(test_get_initial_value<T>);
  TEST(test_set_value<T>);
  TEST(test_reset_value<T>);
  TEST(test_set_value_outofboundexception<T>);
  TEST(test_get_value_outofboundexception<T>);
}

auto main() -> int {
  RUN_SUITE(basic_suite<float>);
  RUN_SUITE(basic_suite<double>);
  RUN_SUITE(accessor_suite<float>);
  RUN_SUITE(accessor_suite<double>);
  return 0;
}