#include "SpMV.hpp"
#include "SparseMatrix_CSR.hpp"

#include <cmath>
#include <cstddef>

// Include the testing framework after all other headers.
#include "unit_test_framework.hpp"

namespace
{
    template <typename T>
    void populateMatrix(SpMV::SparseMatrix_CSR<T> &matrix)
    {
        // jumble the order so assemble() actually sorts
        matrix.setValue(2, 1, static_cast<T>(6.25));
        matrix.setValue(0, 0, static_cast<T>(2.5));
        matrix.setValue(0, 3, static_cast<T>(-4.0));
    }

    template <typename T>
    void populateIdentity(SpMV::SparseMatrix_CSR<T> &matrix)
    {
        for (std::size_t i = 0; i < matrix.getNrows(); i++) {
            matrix.setValue(i, i, static_cast<T>(1.0));
        }
    }

    template <typename T>
    std::size_t csrBytes(const SpMV::SparseMatrix_CSR<T> &A)
    {
        const std::size_t m = A.getNrows();
        const std::size_t nnz = A.getNumnz();
        return (m + 1) * sizeof(std::size_t) // _ia
               + nnz * sizeof(std::size_t)   // _ja
               + nnz * sizeof(T);            // _aij
    }
}

template <typename T>
TEST_CASE(rowptr)
{
    SpMV::SparseMatrix_CSR<T> matrix(3, 4);
    populateMatrix(matrix);
    matrix.assemble();

    const std::size_t *ia = matrix.getRowPtr();
    const std::size_t expected[] = {0, 2, 2, 3};
    for (std::size_t i = 0; i <= matrix.getNrows(); i++) {
        ASSERT(ia[i] == expected[i]);
    }

    // CSR invariants: starts at 0, ends at nnz, non-decreasing
    ASSERT(ia[0] == 0);
    ASSERT(ia[matrix.getNrows()] == matrix.getNumnz());
    for (std::size_t i = 0; i < matrix.getNrows(); i++) {
        ASSERT(ia[i] <= ia[i + 1]);
    }
}

template <typename T>
TEST_CASE(col_idx)
{
    SpMV::SparseMatrix_CSR<T> matrix(3, 4);
    populateMatrix(matrix);
    matrix.assemble();

    const std::size_t *ja = matrix.getColIdx();
    const std::size_t expected[] = {0, 3, 1};
    for (std::size_t k = 0; k < matrix.getNumnz(); k++) {
        ASSERT(ja[k] < matrix.getNcols());
        ASSERT(ja[k] == expected[k]);
    }
}

template <typename T>
TEST_CASE(values)
{
    SpMV::SparseMatrix_CSR<T> matrix(3, 4);
    populateMatrix(matrix);
    matrix.assemble();

    const T eps = static_cast<T>(1e-5); // should be ine for both float and double
    const T *aij = matrix.getValues();
    const T expected[] = {static_cast<T>(2.5), static_cast<T>(-4.0),
                          static_cast<T>(6.25)};
    for (std::size_t k = 0; k < matrix.getNumnz(); k++) {
        ASSERT_NEAR(aij[k], expected[k], eps);
    }
}

template <typename T>
TEST_CASE(storage_bytes)
{
    SpMV::SparseMatrix_CSR<T> matrix(3, 4);
    populateMatrix(matrix);
    matrix.assemble();
    ASSERT(csrBytes(matrix) ==
           4 * sizeof(std::size_t) + 3 * sizeof(std::size_t) + 3 * sizeof(T));
}

template <typename T>
TEST_CASE(identity_rowptr)
{
    SpMV::SparseMatrix_CSR<T> matrix(4, 4);
    populateIdentity(matrix);
    matrix.assemble();

    // One entry per row, so row i starts at position i
    const std::size_t *ia = matrix.getRowPtr();
    for (std::size_t i = 0; i <= matrix.getNrows(); i++) {
        ASSERT(ia[i] == i);
    }
}

template <typename T>
TEST_CASE(identity_col_idx)
{
    SpMV::SparseMatrix_CSR<T> matrix(4, 4);
    populateIdentity(matrix);
    matrix.assemble();

    // Diagonal entry k sits in column k
    const std::size_t *ja = matrix.getColIdx();
    for (std::size_t k = 0; k < matrix.getNumnz(); k++) {
        ASSERT(ja[k] == k);
    }
}

template <typename T>
TEST_CASE(identity_values)
{
    SpMV::SparseMatrix_CSR<T> matrix(4, 4);
    populateIdentity(matrix);
    matrix.assemble();

    const T eps = static_cast<T>(1e-5);
    const T *aij = matrix.getValues();
    ASSERT(matrix.getNumnz() == 4);
    for (std::size_t k = 0; k < matrix.getNumnz(); k++) {
        ASSERT_NEAR(aij[k], static_cast<T>(1.0), eps);
    }
}

template <typename T>
TEST_SUITE(csr_assembly_suite)
{
    TEST(rowptr<T>);
    TEST(col_idx<T>);
    TEST(values<T>);
    TEST(storage_bytes<T>);
    TEST(identity_rowptr<T>);
    TEST(identity_col_idx<T>);
    TEST(identity_values<T>);
}

auto main() -> int
{
    RUN_SUITE(csr_assembly_suite<float>);
    RUN_SUITE(csr_assembly_suite<double>);
    return 0;
}
