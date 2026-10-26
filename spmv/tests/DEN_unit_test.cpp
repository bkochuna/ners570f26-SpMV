
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

TEST_SUITE(DEN_suite)
{
    TEST(test_DEN_class);
}

auto main() -> int
{
    RUN_SUITE(DEN_suite);
    return 0;
}
