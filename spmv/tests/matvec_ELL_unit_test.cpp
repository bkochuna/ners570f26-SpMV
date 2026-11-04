#include "SpMV.hpp"

#include <vector> // std::vector
#include <random>

// Testing library required for testing (Always include this last!)
#include "unit_test_framework.hpp"

// Use ASSERT(condition) to test if a condition is true.
// Direct comparison of floating point numbers is not recommended, so we define
// ASSERT_NEAR(a, b, epsilon) to test if a and b are within epsilon of each
// other.

// At this point, someone has already implemented matvec for ELL so we call that
// method and check that it works correctly.  We will use the definition of matrix-
// vector multiplication to compute the expected result.

// random number generator for generating test matrices and vectors
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<int> prefactor(0,1); // 0 or 1
std::uniform_int_distribution<int> value(-100,100); // [-100, 100]

double epsilon = 1e-6; // tolerance for floating point comparisons


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
            O_ELL.setValue(i, j, O[i][j]);
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
            I_ELL.setValue(i, j, I[i][j]);
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


template <size_t nrows, size_t ncols, typename T>
TEST_CASE(arbitraryMat) 
{

    T A[nrows][ncols] = {}; // arbitrary matrix of size nrows x ncols
    T x[ncols] = {}; // input vector for matvec
    T b[ncols] = {}; // expected result of A*x

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
            A_ELL.setValue(i, j, A[i][j]);
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


template <typename T>
TEST_SUITE(matvec_suite) 
{
    TEST((nullMat<10, T>));
    TEST((identityMat<10, T>));
    TEST((arbitraryMat<25,25, T>)); // square matrix
    TEST((arbitraryMat<5,25, T>)); // short, wide matrix
    TEST((arbitraryMat<25,5, T>)); // tall, skinny matrix
} // matvec_suite


auto
main() -> int
{
  // Run the unit tests. If a test fails, the program will print failure info
  // and return 1.
  RUN_SUITE(matvec_suite<int>);
  RUN_SUITE(matvec_suite<double>);
  RUN_SUITE(matvec_suite<float>);
  return 0; 
}