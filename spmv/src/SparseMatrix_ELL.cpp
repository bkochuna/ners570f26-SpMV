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
        // This routine converts _buildCoeff into the ELL format.
        assert(this->_state == MatrixState::building);
        const size_t nrows = this->_nrows;

        // Count the entries in each row. The longest row sets the padded width
        // that every row is stored with.
        std::vector<int> rowCount(nrows, 0);
        for (const auto& [ij, val] : this->_buildCoeff)
            rowCount[ij.first]++;
        
        max_length_row = 0;
        for (size_t c : rowCount)
            max_length_row = std::max(max_length_row, c);

        // Allocate with the padding already in place.
        col_idx.assign(nrows * max_length_row, 0);
        values.assign(nrows * max_length_row, fp_type(0));

        // Finally, place each entry properly. Most of this was copied
        // from my solution in the previous homework, but just changed so it
        // will work here. 
        // next[i] tracks how many entries of each row have been placed so far,
        // so the k-th entry of row i goes to the column-major index.
        std::vector<int> next(nrows, 0);
        for (const auto& [ij, val] : this->_buildCoeff) {
            const size_t i = ij.first;
            const size_t idx = next[i] * nrows + i;
            col_idx[idx] = ij.second;
            values[idx]  = val;
            next[i]++;
        }

        // Now the ELL-associated arrays hold all of the data, so we can clear
        // everything else.
        this->_numnz = _buildCoeff.size();
        this->_buildCoeff.clear();
        
        this->_state = MatrixState::assembled;
        assert(this->_state == MatrixState::assembled);
    }

    template <class fp_type>
    void SparseMatrix_ELL<fp_type>::disassemble()
    {
        // This routine reverts the ELL storage format to _buildCoeff.
        assert(this->_state == MatrixState::assembled);
        const size_t nrows = this->_nrows;

        // Copy every entry back into the map. Padding slots will have a value
        // of 0, so we'll skip all of those.
        this->_buildCoeff.clear();
        for (size_t i = 0; i < max_length_row; i++) {
            for (size_t j = 0; j < nrows; j++) {
                const size_t idx = i * nrows + j;
                if (values[idx] == fp_type(0)) continue;
                this->_buildCoeff[{i, col_idx[idx]}] = values[idx];
            }
        }
        this->_numnz = this->_buildCoeff.size();

        // Release the ELL arrays that were assembled. We're going to assign
        // a new vector rather than clear()-ing as this actually frees up 
        // memory as opposed to keeping it allocated.
        col_idx = std::vector<size_t>();
        values = std::vector<fp_type>();
        max_length_row = 0;

        this->_state = MatrixState::building;
        assert(this->_state == MatrixState::building);
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
}

// Need to declare the concrete templates within the library for
// use in code that links to libspmv
template class SpMV::SparseMatrix_ELL<float>;
template class SpMV::SparseMatrix_ELL<double>;
