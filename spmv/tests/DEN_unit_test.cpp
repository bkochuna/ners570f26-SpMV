#include "SpMV.hpp"

#include <vector> // std::vector

// Testing library required for testing (Always include this last!)
#include "unit_test_framework.hpp"

// Use ASSERT(condition) to test if a condition is true.
// Direct comparison of floating point numbers is not recommended, so we define
// ASSERT_NEAR(a, b, epsilon) to test if a and b are within epsilon of each
// other.

TEST_CASE(get_and_setMatValue)
{
    size_t nRows = 3;
    size_t nCols = 3;
    SpMV::SparseMatrix_DEN<double> testMat1(nRows,nCols);

    double testVal1 = 1.6592;
    int testRow1 = 2;
    int testCol1 = 2;
    testMat1.setValue(testRow1,testCol1,testVal1);
    ASSERT_NEAR(testMat1.getValue(testRow1,testCol1), testVal1, 1E-3);

    SpMV::SparseMatrix_DEN<double> testMat2(nRows,nCols);
    float testVal2 = 44444.44444;
    int testRow2 = 0;
    int testCol2 = 1;//due to encapsulation, we have to test both at once
    testMat2.setValue(testRow2,testCol2,testVal2);
    ASSERT_NEAR(testMat2.getValue(testRow2,testCol2), testVal2, 1E-3);
}

TEST_CASE(getMatState)
{
    size_t nRows = 1;
    size_t nCols = 1;
    SpMV::SparseMatrix_DEN<double> testMat(nRows,nCols);
    ASSERT(testMat.getState() == MatrixState::initialized);

    testMat.setValue(0,0,1.0);
    ASSERT(testMat.getState() == MatrixState::building);

    testMat.assemble();
    ASSERT(testMat.getState() == MatrixState::assembled);
}

TEST_CASE(getMatRows_and_cols)
{
    size_t nRows = 99;
    size_t nCols = 65;
    SpMV::SparseMatrix_DEN<double> testMat(nRows,nCols);

    ASSERT(testMat.getNrows() == nRows);
    ASSERT(testMat.getNcols() == nCols);
}

TEST_CASE(getMatFormat)
{
    SpMV::SparseMatrix_DEN<double> testMat(1,1);

    ASSERT(testMat.getFormat() == MatrixType::DEN);
    ASSERT(testMat.printFormat() == "DEN");
}


TEST_SUITE(accessors)
{
    TEST(get_and_setMatValue);
    TEST(getMatState);
    TEST(getMatRows_and_cols);
    TEST(getMatFormat);
}
