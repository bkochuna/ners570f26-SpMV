#include "SpMV.hpp"
#include "SparseMatrix.hpp"
#include "SparseMatrix_JDS.hpp"
//#include <gtest/gtest.h> commented because not sure if independent set/get tests are needed and do not totally understand how to do this

// Testing library required for testing (Always include this last!)
#include "unit_test_framework.hpp"

// Need these for viewing
#include <sstream>
#include <string>

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
  ASSERT(test.getFormat() == SpMV::MatrixType::JDS);
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

// Test that the JDS matrix view displays readable information - Steven
template <typename T> TEST_CASE(test_view)
{
    SpMV::SparseMatrix_JDS<T> test(3, 3);

    test.setValue(0, 0, T{1.0});
    test.setValue(0, 2, T{2.0});
    test.setValue(1, 1, T{3.0});

    test.assemble();

    // Capture the view output in a string stream
    std::ostringstream output;
    test.view(output);

    // Check that the output contains readable matrix information
    std::string result = output.str();

    ASSERT(result.find("JDS matrix") != std::string::npos);
    ASSERT(result.find("Dimensions: 3 x 3") != std::string::npos);
    ASSERT(result.find("Nonzeros: 3") != std::string::npos);
    ASSERT(result.find("State: assembled") != std::string::npos);
    ASSERT(result.find("row_srt_idx") != std::string::npos);
    ASSERT(result.find("row_map") != std::string::npos);
    ASSERT(result.find("iter_ptr:") != std::string::npos);
    ASSERT(result.find("col_idx:") != std::string::npos);
    ASSERT(result.find("value:") != std::string::npos);
}

// Basic Test Suite
template <typename T> TEST_SUITE(basic_suite) {
  TEST(test_initialization<T>);
  TEST(test_get_format<T>);
  TEST(test_state<T>);
  TEST(test_dim<T>);
  TEST(test_view<T>);
}

// Accessor Test cases
template <typename T> TEST_CASE(test_get) {
  SpMV::SparseMatrix_JDS<T> test(3, 3);
  // A = [1 0 2.35] example matrix used for all accessor tests
  //     [0 -3 0]
  //     [4.1 5 -6]
  test.setValue(0, 0, static_cast<T>(1));
  test.setValue(0, 2, static_cast<T>(2.35));
  test.setValue(1, 1, static_cast<T>(-3));
  test.setValue(2, 0, static_cast<T>(4.1));
  test.setValue(2, 1, static_cast<T>(5));
  test.setValue(2, 2, static_cast<T>(-6));
  test.assemble();

  ASSERT_NEAR(static_cast<double>(test.getValue(0,0)),static_cast<double>(1),1e-3);
  ASSERT_NEAR(static_cast<double>(test.getValue(0,2)),static_cast<double>(2.35),1e-3);
  ASSERT_NEAR(static_cast<double>(test.getValue(1,1)),static_cast<double>(-3),1e-3);
  ASSERT_NEAR(static_cast<double>(test.getValue(2,0)),static_cast<double>(4.1),1e-3);
  ASSERT_NEAR(static_cast<double>(test.getValue(2,1)),static_cast<double>(5),1e-3);
  ASSERT_NEAR(static_cast<double>(test.getValue(2,2)),static_cast<double>(-6),1e-3);
}

template <typename T> TEST_CASE(test_set) {
  SpMV::SparseMatrix_JDS<T> test(3, 3);
  test.setValue(0, 0, static_cast<T>(1));
  test.setValue(0, 2, static_cast<T>(2.35));
  test.setValue(1, 1, static_cast<T>(-3));
  test.setValue(2, 0, static_cast<T>(4.1));
  test.setValue(2, 1, static_cast<T>(5));
  test.setValue(2, 2, static_cast<T>(-6));
  test.assemble();

  test.setValue(0,0,static_cast<T>(5.7));
  test.assemble();
  ASSERT_NEAR(static_cast<double>(test.getValue(0,0)),static_cast<double>(5.7),1e-3);
  test.setValue(0,2,static_cast<T>(-.1));
  test.assemble();
  ASSERT_NEAR(static_cast<double>(test.getValue(0,2)),static_cast<double>(-.1),1e-3);
}
// Included in DBC for setValue
// template <typename T> TEST_CASE(test_set_value_outofboundexception) {
//   SpMV::SparseMatrix_JDS<T> test(3, 3);
//   test.setValue(0, 0, T{1});
//   test.setValue(0, 2, T{2.35});
//   test.setValue(1, 1, T{-3});
//   test.setValue(2, 0, T{4.1});
//   test.setValue(2, 1, T{5});
//   test.setValue(2, 2, T{-6});
//   test.assemble();

//   bool exceptionThrown = false;
//   try
//   {
//       test.setValue(4,4,1);
//   }
//   catch(ExceptionType&) // special exception type
//   {
//       exceptionThrown = true;
//   }
//   catch(...) // or any exception at all
//   {
//       exceptionThrown = true;
//   }
//   ASSERT(exceptionThrown); 
// }

// Included in DBC for getValue
// template <typename T> TEST_CASE(test_get_value_outofboundexception) {
//   SpMV::SparseMatrix_JDS<T> test(3, 3);
//   test.setValue(0, 0, T{1});
//   test.setValue(0, 2, T{2.35});
//   test.setValue(1, 1, T{-3});
//   test.setValue(2, 0, T{4.1});
//   test.setValue(2, 1, T{5});
//   test.setValue(2, 2, T{-6});
//   test.assemble();

//   bool exceptionThrown = false;
//   try
//   {
//       test.getValue(4,4,3);
//   }
//   catch(ExceptionType&) // special exception type
//   {
//       exceptionThrown = true;
//   }
//   catch(...) // or any exception at all
//   {
//       exceptionThrown = true;
//   }
//   ASSERT(exceptionThrown); 
// }

template <typename T> TEST_CASE(test_get_value_empty) {
  SpMV::SparseMatrix_JDS<T> test(3, 3);
  test.assemble();
  ASSERT_NEAR(static_cast<double>(test.getValue(0,0)), static_cast<double>(0), 1e-3); 
}

template <typename T> TEST_SUITE(accessor_suite) {
  TEST(test_get<T>);
  TEST(test_set<T>);
  // TEST(test_set_value_outofboundexception<T>);
  // TEST(test_get_value_outofboundexception<T>);
  TEST(test_get_value_empty<T>);
}

// Assembly/disassembly unit tests - Hanga

template <typename T>
TEST_CASE(nominalAssembleDisassemble)
{
    // testing the nominal use case
    // using a predefined 5x5 lower triangular matrix
    // except for an empty row at row index 3
    size_t m = 5; // nrows
    size_t n = 5; // ncols

    // create JDS matrix instance
    SpMV::SparseMatrix_JDS<T> test_matrix(m, n);

    // matrix entry - starts at 1.0
    T mij = 1.0;
    // matrix entry taken from assembled matrix later
    T test_mij = 0.0;

    // set test matrix values
    for (size_t i=0; i<m; i++)
    {
        if (i!=3)
        {
            for (size_t j=0; j<=i; j++)
            {
                test_matrix.setValue(i, j, mij);
                mij += static_cast<T>(1.0);
            }

        }
    }

    // assemble matrix
    test_matrix.assemble();

    // see if it assembled
    ASSERT(test_matrix.getState() == SpMV::MatrixState::assembled);

    // test if values are what they should be
    mij = 1.0;
    for (size_t i=0; i<5; i++)
    {
        if (i!=3)
        {
            for (size_t j=0; j<n; j++)
            {
                // make sure nonzero values are where they should be
                // and are what they should be
                if (j<=i)
                {
                    test_mij = test_matrix.getValue(i, j);
                    ASSERT_NEAR(mij, test_mij, static_cast<T>(1e-3));
                    mij += static_cast<T>(1.0);
                }
                // also test that values that weren't explicitly set are zero
                else
                {
                    test_mij = test_matrix.getValue(i, j);
                    ASSERT_NEAR(static_cast<T>(0.0), test_mij, static_cast<T>(1e-3));
                }
            }

        }
        else // testing that the empty row is full of only zeros
        {
            for (size_t j=0; j<n; j++)
            {
                test_mij = test_matrix.getValue(i, j);
                ASSERT_NEAR(static_cast<T>(0.0), test_mij, static_cast<T>(1e-3));
            }
        }
    }

    // disassemble matrix
    test_matrix.disassemble();
    ASSERT(test_matrix.getState() == SpMV::MatrixState::building);
    // test matrix will be destroyed automatically once out of scope
}

template <typename T>
TEST_CASE(assembleImpliedDisassemble)
{
    // testing if disassemble works when setValue called
    // just using a simple 2x2 identity matrix for this one

    size_t m = 2; // nrows & ncols

    // create JDS matrix instance
    SpMV::SparseMatrix_JDS<T> id_matrix(m, m);

    // set identity matrix values
    for (size_t i=0; i<m; i++)
    {
        id_matrix.setValue(i, i, static_cast<T>(1.0));
    }

    // assemble matrix
    id_matrix.assemble();
    // now call setValue on very first element
    id_matrix.setValue(0, 0, static_cast<T>(-1.0));
    // and check matrix state
    ASSERT(id_matrix.getState() == SpMV::MatrixState::building);
    // test matrix will be destroyed automatically once out of scope
}

template <typename T>
TEST_CASE(emptyAssembleDisassemble)
{
    // make 2x2 test matrix
    SpMV::SparseMatrix_JDS<T> test_matrix(2,2);
    // assemble it just to make sure it doesn't break
    // making sure its values are zero is handled by test_get_value_empty case
    test_matrix.assemble();
    // disassemble it to make sure it doesn't break
    test_matrix.disassemble();
    
}


template <typename T>
TEST_SUITE(assemble_suite)
{
    TEST(nominalAssembleDisassemble<T>);
    TEST(assembleImpliedDisassemble<T>);
    TEST(emptyAssembleDisassemble<T>);
}


auto main() -> int {
  RUN_SUITE(basic_suite<float>);
  RUN_SUITE(basic_suite<double>);
  RUN_SUITE(accessor_suite<float>);
  RUN_SUITE(accessor_suite<double>);
  RUN_SUITE(assemble_suite<float>);
  RUN_SUITE(assemble_suite<double>);
  return 0;
}