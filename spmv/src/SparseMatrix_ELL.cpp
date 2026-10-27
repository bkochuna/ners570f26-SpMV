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

    std::vector<double> SparseMatrix_ELL<fp_type>::to_dense() const {
        std::vector<double> dense(A.nrows * A.ncols, 0.0); // transform ell into dense format
        for (int r = 0; r < nrows; ++r) {
            int offset = r * A.max_nnz_per_row;
            for (int k = 0; k < A.max_nnz_per_row; ++k) {
                int c = A.col_idx[offset + k];
                if (c == -1) continue; 
                dense[r * A.ncols + c] = A.values[offset + k];
            }
        }
        return dense;
}
}

// Need to declare the concrete templates within the library for
// use in code that links to libspmv
template class SpMV::SparseMatrix_ELL<float>;
template class SpMV::SparseMatrix_ELL<double>;
