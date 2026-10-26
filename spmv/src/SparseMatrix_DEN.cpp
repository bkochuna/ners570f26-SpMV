#include "SparseMatrix_DEN.hpp"

#include <iostream>
#include <stddef.h>

#include <vector> // std::vector
#include <cassert> // assert

using namespace std;

namespace SpMV
{
    template <class fp_type>
    SparseMatrix_DEN<fp_type>::SparseMatrix_DEN(const size_t nrows, const size_t ncols) :
        SparseMatrix<fp_type>::SparseMatrix(nrows, ncols)
    {
        cout << "Hello from DEN Constructor" << endl;
    }

    template <class fp_type>
    void SparseMatrix_DEN<fp_type>::assemble()
    {
        cout << "Hello from DEN assemble" << endl;
        assert(this->_state == MatrixState::building);
        
        //This routine needs to convert _buildCoeff into the DEN storage format.
        this->_a.assign(
            this->_nrows,
            std::vector<fp_type>(this->_ncols, 0.0)
        );
        for (const auto& entry: this->_buildCoeff) {
            size_t i = entry.first.first;
            size_t j = entry.first.second;
            fp_type val = entry.second;

            this->_a[i][j] = val;
        }

        this->_state = MatrixState::assembled;
        assert(this->_state == MatrixState::assembled);

    }

    template <class fp_type>
    void SparseMatrix_DEN<fp_type>::disassemble()
    {
        cout << "Hello from DEN disassemble" << endl;
        assert(this->_state == MatrixState::assembled);

        //This routine needs to revert DEN storage format to _buildCoeff
        this->_buildCoeff.clear();
        for (size_t i = 0; i < this->_nrows; i++) {
            for (size_t j = 0; j < this->_ncols; j++) {
                fp_type val = this->_a[i][j];
                this->_buildCoeff[std::make_pair(i,j)] = val;
            }
        }

        this->_state = MatrixState::building;
        assert(this->_state == MatrixState::building);
    }

    template <class fp_type>
    fp_type SparseMatrix_DEN<fp_type>::getValue(const size_t i, const size_t j) const noexcept
    {
        assert(this->_state > MatrixState::initialized);
        assert(i < this->_nrows);
        assert(j < this->_ncols);

        // Assembled: read directly from the dense storage
        if (this->_state == MatrixState::assembled)
            return this->_a[i][j];

        // Building: read from the build coefficients (unset entries are zero)
        const auto it = this->_buildCoeff.find(std::make_pair(i, j));
        return (it != this->_buildCoeff.end()) ? it->second : static_cast<fp_type>(0);
    }

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
