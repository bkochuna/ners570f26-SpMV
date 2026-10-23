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

    template <class fp_type>
    void SparseMatrix_ELL<fp_type>::view()
    {
        assert(this->_state == MatrixState::assembled);

        size_t nrows = this->_nrows;
        size_t maxnz_row = this->_maxnz_row;

        std::ofstream outfile("ELL.view");

        for (size_t i=0; i<nrows*maxnz_row; i++)
        {
            outfile << std:setw(10) << this->_ja[i] << ' ' << this->_vals[i] << '\n';
        }

        outfile.close();
    }


}

// Need to declare the concrete templates within the library for
// use in code that links to libspmv
template class SpMV::SparseMatrix_ELL<float>;
template class SpMV::SparseMatrix_ELL<double>;
