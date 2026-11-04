#include "SpMV.hpp"
#include "SparseMatrix.hpp"
#include "SparseMatrix_ELL.hpp"

#include <cstddef>
#include <vector>
#include <random>

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


// ============================================================================
// ==== MATVEC TESTS: Implemented by alex-tuckey
// ============================================================================
std::random_device rd;
std::mt19937 gen(rd());

double epsilon = 1e-6; // tolerance for floating point comparisons

// Test 1: Null matrix of size N x N
template <size_t N, typename T>
TEST_CASE(nullMat) 
{
    T O[N][N] = {}; // null matrix of size N x N
    T x[N] = {}; // input vector for matvec
    T b[N] = {}; // expected result of O*x

    for (size_t i=0; i<N; i++) 
    {
       for (size_t j=0; j<N; j++) 
       {
           O[i][j] = static_cast<T>(0.0); // set all elements to 0
       }
    }

    std::uniform_real_distribution<T> value(-100,100); // [-100, 100]

    for (size_t i=0; i<N; i++) 
    {
        x[i] = static_cast<T>(value(gen)); // arbitrary values for x
        b[i] = static_cast<T>(0.0); // expected result of O*x is 0
    }

    // create a SparseMatrix_ELL object and set the values of O
    SpMV::SparseMatrix_ELL<T> O_ELL(N,N);
    for (size_t i=0; i<N; i++) 
    {
        for (size_t j=0; j<N; j++) 
        {
            O_ELL.set_values(i, j, O[i][j]);
        }
    }

    T b_ELL = O_ELL.matvec(x); // result of O*x

    // check that the result has the correct size
    ASSERT(b_ELL.size() == b.size());

    // check that the result is correct
    for (size_t i=0; i<b_ELL.size(); i++) 
    {
        ASSERT_NEAR(b_ELL[i], b[i], epsilon);
    }

} // nullMat


// Test 2: Identity matrix of size N x N
template <size_t N, typename T>
TEST_CASE(identityMat) 
{
    T I[N][N] = {}; // identity matrix of size N x N
    T x[N] = {}; // input vector for matvec
    T b[N] = {}; // expected result of I*x

    for (size_t i=0; i<N; i++) 
    {
       for (size_t j=0; j<N; j++) 
       {
           if (i==j) 
           {
               I[i][j] = static_cast<T>(1.0); // set diagonal elements to 1
           }
           else 
           {
               I[i][j] = static_cast<T>(0.0); // set all other elements to 0
           }
       }
    }

    std::uniform_real_distribution<T> value(-100,100); // [-100, 100]

    for (size_t i=0; i<N; i++) 
    {
        x[i] = static_cast<T>(value(gen)); // arbitrary values for x
        b[i] = x[i]; // expected result of I*x is x
    }

    // create a SparseMatrix_ELL object and set the values of I
    SpMV::SparseMatrix_ELL<T> I_ELL(N,N);
    for (size_t i=0; i<N; i++) 
    {
        for (size_t j=0; j<N; j++) 
        {
            I_ELL.set_values(i, j, I[i][j]);
        }
    }

    T b_ELL = I_ELL.matvec(x); // result of I*x

    // check that the result has the correct size
    ASSERT(b_ELL.size() == b.size());

    // check that the result is correct
    for (size_t i=0; i<b_ELL.size(); i++) 
    {
        ASSERT_NEAR(b_ELL[i], b[i], epsilon);
    }

} // identityMat


// Test 3: Arbitrary matrix of size nrows x ncols
template <size_t nrows, size_t ncols, typename T>
TEST_CASE(arbitraryMat) 
{

    T A[nrows][ncols] = {}; // arbitrary matrix of size nrows x ncols
    T x[ncols] = {}; // input vector for matvec
    T b[ncols] = {}; // expected result of A*x

    std::uniform_int_distribution<int> prefactor(0,1); // 0 or 1
    std::uniform_real_distribution<T> value(-100,100); // [-100, 100]

    for (size_t i=0; i<nrows; i++) 
    {
        for (size_t j=0; j<ncols; j++) 
        {
                A[i][j] = static_cast<T>(prefactor(gen) * value(gen)); // arbitrary value or zero
        }
    }

    for (size_t j=0; j<ncols; j++) 
    {
        x[j] = static_cast<T>(value(gen)); // arbitrary values for x
    }

    for (size_t i=0; i<nrows; i++) 
    {
        for (size_t j=0; j<ncols; j++) 
        {
            b[i] = b[i] + A[i][j] * x[j]; // compute the result of A*x
        }
    }

    // create a SparseMatrix_ELL object and set the values of A
    SpMV::SparseMatrix_ELL<T> A_ELL(nrows, ncols);
    for (size_t i=0; i<nrows; i++) 
    {
        for (size_t j=0; j<ncols; j++) 
        {
            A_ELL.set_values(i, j, A[i][j]);
        }
    }

    T b_ELL = A_ELL.matvec(x); // result of A*x

    // check that the result has the correct size
    ASSERT(b_ELL.size() == b.size());

    // check that the result is correct
    for (size_t i=0; i<b_ELL.size(); i++) 
    {
        ASSERT_NEAR(b_ELL[i], b[i], epsilon);
    }

} // arbitraryMat


// Matvec test suite
template <typename T>
TEST_SUITE(matvec_suite) 
{
    TEST((nullMat<10, T>));
    TEST((identityMat<10, T>));
    TEST((arbitraryMat<25,25, T>)); // square matrix
    TEST((arbitraryMat<5,25, T>)); // short, wide matrix
    TEST((arbitraryMat<25,5, T>)); // tall, skinny matrix
} // matvec_suite




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

    RUN_SUITE(matvec_suite<float>);
    RUN_SUITE(matvec_suite<double>);

    return 0;
}