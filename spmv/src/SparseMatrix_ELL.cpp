#include "SparseMatrix_ELL.hpp"

#include <iostream>
#include <stddef.h>

using namespace std;

namespace SpMV
{
    template <class fp_type>
    SparseMatrix_ELL<fp_type>::SparseMatrix_ELL(const size_t nrows, const size_t ncols) :
        SparseMatrix<fp_type>::SparseMatrix(nrows, ncols)
    {
        cout << "Hello from ELL Constructor" << endl;
    }

    template <class fp_type>
    void SparseMatrix_ELL<fp_type>::assemble()
    {
        cout << "Hello from ELL assemble" << endl;

        //This routine needs to convert _buildCoeff into the COO storage format.
    }

    template <class fp_type>
    MatrixType SparseMatrix_ELL<fp_type>::getFormat() const noexcept
    {
        return MatrixType::ELL;
    }

    template <class fp_type>
    std::string_view SparseMatrix_ELL<fp_type>::printFormat() const noexcept
    {
        return "ELL";
    }

    // I used the attributes: nrows, width = max_nnz_per_row, col_indices, and values that I expect to get from the A constructed in ELL format
    std::vetcor<double> SparseMatrix_ELL<fp_type>::matvec(const SparseMatrix_ELL& A, const std::vector<double>& x) {
        // declare and initialize output vector y
        std::vector<double> y(A.nrows, 0.0);

        // solve
        for (size_t i = 0; i < A.nrows; ++i) {
            for (size_t k = 0; k < A.width; ++k) {
                const size_t p = i * A.width + k;
                y[i] += A.values[p] * x[A.col_indices[p]];
            }
        }  
        return y;
    }

}

// Need to declare the concrete templates within the library for
// use in code that links to libspmv
template class SpMV::SparseMatrix_ELL<float>;
template class SpMV::SparseMatrix_ELL<double>;
