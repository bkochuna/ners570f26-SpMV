#include "SpMV.hpp"
#include "SparseMatrix_JDS.hpp"

#include <cmath>
#include <cstddef>
#include <limits>
#include <vector>

// Include the test framework after all other headers.
#include "unit_test_framework.hpp"

// Check multiplication when row lengths differ.
template <typename T>
TEST_CASE(matvec_uneven_rows)
{
    SpMV::SparseMatrix_JDS<T> matrix(3, 3);

    // A = [1 0 2]
    //     [0 3 0]
    //     [4 5 6]
    // Insert only the nonzero entries.
    matrix.setValue(0, 0, T{1});
    matrix.setValue(0, 2, T{2});
    matrix.setValue(1, 1, T{3});
    matrix.setValue(2, 0, T{4});
    matrix.setValue(2, 1, T{5});
    matrix.setValue(2, 2, T{6});

    // Convert the construction data into JDS storage.
    matrix.assemble();

    const std::vector<T> input = {T{1}, T{2}, T{3}};
    const std::vector<T> expected = {T{7}, T{6}, T{32}};

    // The default argument requests one multiplication.
    const std::vector<T> actual = matrix.matvec(input);

    // Check the length before accessing output entries.
    ASSERT(actual.size() == expected.size());

    const T tolerance = T{64} * std::numeric_limits<T>::epsilon();

    // Check results in the original matrix row order.
    for (std::size_t row = 0; row < expected.size(); ++row)
    {
        ASSERT_NEAR(actual[row], expected[row], tolerance);
    }
}

// Check that an empty row produces a zero result.
template <typename T>
TEST_CASE(matvec_empty_row)
{
    SpMV::SparseMatrix_JDS<T> matrix(3, 3);

    // A = [1 0 2]
    //     [0 0 0]
    //     [4 5 6]
    // Leave row 1 empty.
    matrix.setValue(0, 0, T{1});
    matrix.setValue(0, 2, T{2});
    matrix.setValue(2, 0, T{4});
    matrix.setValue(2, 1, T{5});
    matrix.setValue(2, 2, T{6});

    matrix.assemble();

    const std::vector<T> input = {T{1}, T{2}, T{3}};
    const std::vector<T> expected = {T{7}, T{0}, T{32}};
    const std::vector<T> actual = matrix.matvec(input);

    ASSERT(actual.size() == expected.size());

    const T tolerance = T{64} * std::numeric_limits<T>::epsilon();

    for (std::size_t row = 0; row < expected.size(); ++row)
    {
        ASSERT_NEAR(actual[row], expected[row], tolerance);
    }
}

// Run both cases with the selected floating-point type.
template <typename T>
TEST_SUITE(jds_matvec_suite)
{
    TEST(matvec_uneven_rows<T>);
    TEST(matvec_empty_row<T>);
}

int main()
{
    RUN_SUITE(jds_matvec_suite<float>);
    RUN_SUITE(jds_matvec_suite<double>);
    return 0;
}