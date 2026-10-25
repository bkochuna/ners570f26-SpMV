#include "SpMV.hpp"
#include "SparseMatrix_CSR.hpp"

#include <cmath>
#include <cstddef>

// Include the testing framework after all other headers.
#include "unit_test_framework.hpp"

namespace
{
    template <typename T>
    void populateMatrix(SpMV::SparseMatrix_CSR<T>& matrix)
    {
        matrix.setValue(0, 0, static_cast<T>(2.5));
        matrix.setValue(0, 3, static_cast<T>(-4.0));
        matrix.setValue(2, 1, static_cast<T>(6.25));
    }
}

template <typename T>
TEST_CASE(csr_numnz)
{
    SpMV::SparseMatrix_CSR<T> matrix(3, 4);
    ASSERT(matrix.getNumnz() == 0);

    populateMatrix(matrix);
    ASSERT(matrix.getNumnz() == 3);

    matrix.setValue(0, 0, static_cast<T>(10.5));
    ASSERT(matrix.getNumnz() == 3);

    matrix.assemble();
    ASSERT(matrix.getNumnz() == 3);
}

template <typename T>
TEST_CASE(csr_nrows)
{
    SpMV::SparseMatrix_CSR<T> matrix(3, 4);
    ASSERT(matrix.getNrows() == 3);
    populateMatrix(matrix);
    matrix.assemble();
    ASSERT(matrix.getNrows() == 3);
}

template <typename T>
TEST_CASE(csr_ncols)
{
    SpMV::SparseMatrix_CSR<T> matrix(3, 4);
    ASSERT(matrix.getNcols() == 4);
    populateMatrix(matrix);
    matrix.assemble();
    ASSERT(matrix.getNcols() == 4);
}

template <typename T>
TEST_CASE(csr_format)
{
    SpMV::SparseMatrix_CSR<T> matrix(3, 4);
    ASSERT(matrix.getFormat() == SpMV::MatrixType::CSR);
    ASSERT(matrix.printFormat() == "CSR");
    populateMatrix(matrix);
    matrix.assemble();
    ASSERT(matrix.getFormat() == SpMV::MatrixType::CSR);
    ASSERT(matrix.printFormat() == "CSR");
}

template <typename T>
TEST_CASE(csr_state)
{
    SpMV::SparseMatrix_CSR<T> matrix(3, 4);
    ASSERT(matrix.getState() == SpMV::MatrixState::initialized);

    populateMatrix(matrix);
    ASSERT(matrix.getState() == SpMV::MatrixState::building);

    matrix.assemble();
    ASSERT(matrix.getState() == SpMV::MatrixState::assembled);

    matrix.disassemble();
    ASSERT(matrix.getState() == SpMV::MatrixState::building);
}

template <typename T>
TEST_CASE(csr_single_value)
{
    SpMV::SparseMatrix_CSR<T> matrix(3, 4);
    populateMatrix(matrix);
    const T epsilon = static_cast<T>(1e-5);

    ASSERT_NEAR(matrix.getValue(0, 0), static_cast<T>(2.5), epsilon);
    matrix.setValue(0, 0, static_cast<T>(10.5));
    ASSERT_NEAR(matrix.getValue(0, 0), static_cast<T>(10.5), epsilon);

    // Updating an existing entry must preserve other entries and metadata.
    ASSERT_NEAR(matrix.getValue(0, 3), static_cast<T>(-4.0), epsilon);
    ASSERT_NEAR(matrix.getValue(2, 1), static_cast<T>(6.25), epsilon);
    ASSERT(matrix.getNumnz() == 3);
    ASSERT(matrix.getNrows() == 3);
    ASSERT(matrix.getNcols() == 4);
    ASSERT(matrix.getState() == SpMV::MatrixState::building);

    matrix.assemble();
    ASSERT_NEAR(matrix.getValue(0, 0), static_cast<T>(10.5), epsilon);
}

template <typename T>
TEST_CASE(csr_multiple_values)
{
    SpMV::SparseMatrix_CSR<T> matrix(3, 4);
    populateMatrix(matrix);
    const T epsilon = static_cast<T>(1e-5);

    const std::size_t rows[] = {0, 0, 2};
    const std::size_t cols[] = {0, 3, 1};
    const T initial[] = {static_cast<T>(2.5), static_cast<T>(-4.0),
                         static_cast<T>(6.25)};
    const T expected[] = {static_cast<T>(2.5), static_cast<T>(-7.5),
                          static_cast<T>(8.25)};

    for (std::size_t k = 0; k < 3; ++k)
    {
        ASSERT_NEAR(matrix.getValue(rows[k], cols[k]), initial[k], epsilon);
    }

    // The current interface exposes scalar accessors. Update two existing
    // entries through that interface, leaving (0, 0) unchanged.
    for (std::size_t k = 1; k < 3; ++k)
    {
        matrix.setValue(rows[k], cols[k], expected[k]);
    }

    for (std::size_t k = 0; k < 3; ++k)
    {
        ASSERT_NEAR(matrix.getValue(rows[k], cols[k]), expected[k], epsilon);
    }
    ASSERT(matrix.getNumnz() == 3);
    ASSERT(matrix.getNrows() == 3);
    ASSERT(matrix.getNcols() == 4);
    ASSERT(matrix.getState() == SpMV::MatrixState::building);

    matrix.assemble();
    matrix.disassemble();
    for (std::size_t k = 0; k < 3; ++k)
    {
        ASSERT_NEAR(matrix.getValue(rows[k], cols[k]), expected[k], epsilon);
    }
    ASSERT(matrix.getNumnz() == 3);
}

template <typename T>
TEST_SUITE(csr_accessor_suite)
{
    TEST(csr_numnz<T>);
    TEST(csr_nrows<T>);
    TEST(csr_ncols<T>);
    TEST(csr_format<T>);
    TEST(csr_state<T>);
    TEST(csr_single_value<T>);
    TEST(csr_multiple_values<T>);
}

auto main() -> int
{
    RUN_SUITE(csr_accessor_suite<float>);
    RUN_SUITE(csr_accessor_suite<double>);
    return 0;
}
