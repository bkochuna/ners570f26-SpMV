#include "SparseMatrix_DEN.hpp"

#include <iostream>
#include <stddef.h>

using namespace std;

namespace SpMV
{
    template <class fp_type>
    SparseMatrix_DEN<fp_type>::SparseMatrix_DEN(const size_t nrows, const size_t ncols) :
        SparseMatrix<fp_type>::SparseMatrix(nrows, ncols),
        _values(_nrows*_ncols, static_cast<fp_type>(0)) //store the dense matrix values
    {}

    template <class fp_type>
    MatrixType SparseMatrix_DEN<fp_type>::getFormat() const noexcept
    {
        return MatrixType::DEN;
    }

    template <class fp_type>
    std::string_view SparseMatrix_DEN<fp_type>::printFormat() const noexcept
    {
        return "DEN";
    }
}

// Need to declare the concrete templates within the library for
// use in code that links to libspmv
template class SpMV::SparseMatrix_DEN<float>;
template class SpMV::SparseMatrix_DEN<double>;
