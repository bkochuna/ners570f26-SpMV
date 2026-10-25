#include "SparseMatrix_JDS.hpp"

#include <iostream>
#include <stddef.h>

using namespace std;

namespace SpMV
{
    template <class fp_type>
    SparseMatrix_JDS<fp_type>::SparseMatrix_JDS(const size_t nrows, const size_t ncols) : 
    SparseMatrix<fp_type>::SparseMatrix(nrows, ncols)
    {
        cout << "Hello from JDS Constructor" << endl;
    }

    template <class fp_type>
    void SparseMatrix_JDS<fp_type>::assemble()
    {
        cout << "Hello from JDS assemble" << endl;

        // This routine needs to convert _buildCoeff into the COO storage format.
    }

    template <class fp_type>
    void SparseMatrix_JDS<fp_type>::disassemble()
    {
        cout << "Hello from JDS disassemble" << endl;
    }

    template <class fp_type>
    MatrixType SparseMatrix_JDS<fp_type>::getFormat() const noexcept
    {
        return MatrixType::JDS;
    }

    template <class fp_type>
    std::string_view SparseMatrix_JDS<fp_type>::printFormat() const noexcept
    {
        return "JDS";
    }
}

// Need to declare the concrete templates within the library for
// use in code that links to libspmv
template class SpMV::SparseMatrix_JDS<float>;
template class SpMV::SparseMatrix_JDS<double>;

