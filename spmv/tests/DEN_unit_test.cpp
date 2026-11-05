#include "SpMV.hpp"

#include <vector> // std::vector

// Testing library required for testing (Always include this last!)
#include "unit_test_framework.hpp"

// Use ASSERT(condition) to test if a condition is true.
// Direct comparison of floating point numbers is not recommended, so we define
// ASSERT_NEAR(a, b, epsilon) to test if a and b are within epsilon of each
// other.

// Create a unit test
TEST_CASE(Identitymatrix) 
{
    SpMV::SparseMatrix_DEN<double> A(3,3);  // Identity matrix of size 3x3
    for (size_t i = 0; i < 3; ++i)  
    {
        for (size_t j = 0; j < 3; ++j)
        {
            if (i == j)
            {
                A.setValue(i,j,1.0);
            }
            else
            {
                A.setValue(i,j,0.0);
            }
        }
    }
    A.assemble(); // The identity matrix is now assembled and ready.

    std::vector<double> x = {1, 2, 3};

    std::vector<double> xnew(3);     
    A.matvec(x, xnew);

    for (size_t i = 0; i < 3; ++i)
    {
        ASSERT_NEAR(xnew[i], x[i], 1e-10); // for floating point comparison.
    }
    
}

template <typename T>
TEST_CASE(negativevalues) 
{
    SpMV::SparseMatrix_DEN<T> A(2,2);  // Matrix of size 2x2
    A.setValue(0,0,1.0);
    A.setValue(0,1,-1.0);
    A.setValue(1,0,-3.0); 
    A.setValue(1,1,2.0);  
    
    A.assemble(); // Matrix is assembled and ready.

    std::vector<T> x = {1.0, -2.0};
    std::vector<T> xnew(2); 
    A.matvec(x, xnew);
    std::vector<T> xref = {3.0, -7.0};
    for (size_t i = 0; i < 2; ++i)
    {
        ASSERT_NEAR(xnew[i], xref[i],static_cast<T>(1e-5));  // 10 × 2 × 1.2e-7 × 7 ≈ 1.7e-5
        // cast keeps the tolerance the same type as the data float or double
    }
    
}

TEST_CASE(onebyone) 
{
    SpMV::SparseMatrix_DEN<double> A(1,1);  // Matrix of size 1x1
    A.setValue(0,0,5.0);
    
    A.assemble(); // Matrix is assembled and ready.

    std::vector<double> x = {2.0};
    std::vector<double> xnew(1);     
    A.matvec(x, xnew);
    std::vector<double> xref = {10.0};
    
    ASSERT(xnew.size() == 1);     // Making sure the output vector of 1x1 matvec is 1x1
    for (size_t i = 0; i < 1; ++i)
    {
        ASSERT_NEAR(xnew[i], xref[i], 1e-7); 
    }
    
}

TEST_CASE(largevalues) 
{
    SpMV::SparseMatrix_DEN<double> A(2,2);  // Matrix of size 2x2
    A.setValue(0,0,1e10);
    A.setValue(0,1,3e10);
    A.setValue(1,0,5e10);
    A.setValue(1,1,2e10);
    A.assemble(); // The matrix is assembled and ready.

    std::vector<double> x= {1e4, 2e4};
    std::vector<double> xnew(2);     
    A.matvec(x, xnew);
    std::vector<double> xref= {7e14, 9e14};
    for (size_t i = 0; i < 2; ++i)
    {
        ASSERT_NEAR(xnew[i], xref[i], 4e0); //tolerance = 4 =10 × 2 × 2.2e-16 × 9e14
    }
    
}
TEST_CASE(smallvalues){
    
    SpMV::SparseMatrix_DEN<double> A(2,2);  // Matrix of size 2x2
    A.setValue(0,0,1e-10);
    A.setValue(0,1,3e-10);
    A.setValue(1,0,5e-10);
    A.setValue(1,1,2e-10);
    A.assemble(); // The matrix is now assembled and ready.

    std::vector<double> x= {1e-4, 2e-4};
    std::vector<double> xnew(2);     
    A.matvec(x, xnew);
    std::vector<double> xref= {7e-14, 9e-14};
    for (size_t i = 0; i < 2; ++i)
    {
        ASSERT_NEAR(xnew[i], xref[i], 4e-28); //tolerance = 10 × 2 × 2.2e-16 x 9e-14=~4e-28 
    }
}




// Issue #32: check every accessor returns the right value for DEN in each
// state (initialized -> building -> assembled).
TEST_CASE(test_DEN_accessors)
{
    SpMV::SparseMatrix_DEN<double> A(3, 4);

    // Initialized: sizes and format are set, nothing stored yet.
    ASSERT(A.getNrows() == 3);
    ASSERT(A.getNcols() == 4);
    ASSERT(A.getNumnz() == 0);
    ASSERT(A.getState() == SpMV::MatrixState::initialized);
    ASSERT(A.getFormat() == SpMV::MatrixType::DEN);
    ASSERT(A.printFormat() == std::string_view("DEN"));

    // Building: getValue reads the values that were set; unset entries are 0.
    A.setValue(0, 1, 2.5);
    A.setValue(2, 3, -1.0);
    ASSERT(A.getState() == SpMV::MatrixState::building);
    ASSERT(A.getNumnz() == 2);
    ASSERT_NEAR(A.getValue(0, 1), 2.5, 1e-12);
    ASSERT_NEAR(A.getValue(1, 1), 0.0, 1e-12);

    // Assembled: getValue reads from the dense storage, other accessors unchanged.
    A.assemble();
    ASSERT(A.getState() == SpMV::MatrixState::assembled);
    ASSERT(A.getNrows() == 3);
    ASSERT(A.getNcols() == 4);
    ASSERT(A.getNumnz() == 2);
    ASSERT(A.getFormat() == SpMV::MatrixType::DEN);
    ASSERT_NEAR(A.getValue(0, 1), 2.5, 1e-12);
    ASSERT_NEAR(A.getValue(2, 3), -1.0, 1e-12);
    ASSERT_NEAR(A.getValue(0, 0), 0.0, 1e-12);

    // getValue is virtual, so calling through a base-class reference uses DEN's version.
    const SpMV::SparseMatrix<double>& B = A;
    ASSERT_NEAR(B.getValue(2, 3), -1.0, 1e-12);
}

TEST_SUITE(DEN_accessor_suite)
{
    TEST(test_DEN_accessors);
}

TEST_SUITE(den_matvec_suite)
{
    TEST(Identitymatrix);
    TEST(negativevalues<float>);
    TEST(negativevalues<double>);
    TEST(onebyone);
    TEST(largevalues);
    TEST(smallvalues);

}

auto
main() -> int
{
  // Run the unit tests. If a test fails, the program will print failure info
  // and return 1.
  RUN_SUITE(den_matvec_suite);
  RUN_SUITE(DEN_accessor_suite);
  return 0; 
}