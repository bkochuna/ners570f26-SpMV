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

    // SETTERS AND GETTERS =============================================================
    template <class fp_type>
    size_t SparseMatrix_ELL<fp_type>::getColIdx(const size_t i, const size_t slot, const size_t totalRows = 0) const
    {
        assert(this->_state > MatrixState::initialized);

        // Access total rows from base class
        if (totalRows == 0) {
            size_t totalRows = this->_nrows;
        }

        // Row-major: (row * max_row_length) + slot
        col_idx = _colidx[(i * _maxrowlength) + slot]

        // Column-major: (slot * total_rows) + row
        //col_idx = _colidx[(i * totalRows) + slot]
        
        return col_idx;
    }

    template <class fp_type>
    fp_type SparseMatrix_ELL<fp_type>::getValue(const size_t i, const size_t slot, const size_t totalRows = 0) const
    {
        assert(this->_state > MatrixState::initialized);

        // Access total rows from base class
        if (totalRows == 0) {
            size_t totalRows = this->_nrows;
        }

        // Row-major: (row * max_row_length) + slot
        val = _vals[(i * _maxrowlength) + slot]

        // Column-major: (slot * total_rows) + row
        //val = _vals[(i * totalRows) + slot]

        return val;
    }
    // =================================================================================

    // FROM WHAT WAS GIVEN TO US:
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
    // ==================================================================================
}

// Need to declare the concrete templates within the library for
// use in code that links to libspmv
template class SpMV::SparseMatrix_ELL<float>;
template class SpMV::SparseMatrix_ELL<double>;
