
#include "SpMV.hpp"

#include "unit_test_framework.hpp"

TEST_CASE(test_DEN_class)
{
    SpMV::SparseMatrix_DEN<double> A(2, 3);

    // Verify the number of rows and columns.
    ASSERT(A.getNrows() == 2);
    ASSERT(A.getNcols() == 3);

    // Verify the initial matrix state.
    ASSERT(A.getState() == SpMV::MatrixState::initialized);
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

TEST_SUITE(DEN_suite)
{
    TEST(test_DEN_class);
    TEST(test_DEN_accessors);
}

auto main() -> int
{
    RUN_SUITE(DEN_suite);
    return 0;
}
