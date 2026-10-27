#include "SparseMatrix.hpp"

#include <iostream>
#include <cassert>

namespace SpMV
{

template <class fp_type>
SparseMatrix<fp_type>::SparseMatrix(const std::size_t nrows, const std::size_t ncols) :
    _nrows(nrows), _ncols(ncols)
{
    assert(this->_state == MatrixState::undefined);
    this->_state = MatrixState::initialized;
    assert(this->_state == MatrixState::initialized);
}

/*
* 
* The header file has 
* `SparseMatrix(const SparseMatrix& that) = default;`
* Here, the ` = default` is equivalent to the following:
*
template <class fp_type>
SparseMatrix<fp_type>::SparseMatrix(const SparseMatrix& that)
    : _state(that._state),
      _nrows(that._nrows),
      _ncols(that._ncols),
      _numnz(that._numnz),
      _buildCoeff(that._buildCoeff)
{
      // Nothing to do in the body
}
* The compiler just generates it automagically.
* This is true for all other copy and move constructors.
*/

template <class fp_type>
SparseMatrix<fp_type>::SparseMatrix(SparseMatrix&& that) noexcept :
    _state(that._state),
    _nrows(that._nrows),
    _ncols(that._ncols),
    _numnz(that._numnz),
    _buildCoeff(std::move(that._buildCoeff))
{
    that._state = MatrixState::undefined;
    that._nrows = 0;
    that._ncols = 0;
    that._numnz = 0;
    that._buildCoeff.clear();
}

template <class fp_type>
SparseMatrix<fp_type>& SparseMatrix<fp_type>::operator=(SparseMatrix&& that) noexcept
{
    if (this != &that) {
        this->_state      = that._state;
        this->_nrows      = that._nrows;
        this->_ncols      = that._ncols;
        this->_numnz      = that._numnz;
        this->_buildCoeff = std::move(that._buildCoeff);

        that._state = MatrixState::undefined;
        that._nrows = 0;
        that._ncols = 0;
        that._numnz = 0;
        that._buildCoeff.clear();
    }

    return *this;
}

template <class fp_type>
SparseMatrix<fp_type>::~SparseMatrix()
{
    // These assignments are not required for destruction and must not 
    // be relied upon to sanitize memory or make access after destruction valid.
    //
    // The preferred definition would be `virtual ~SparseMatrix() = default;`
    //
    this->_state = MatrixState::undefined;
    this->_nrows = 0;
    this->_ncols = 0;
    this->_numnz = 0;
    // Destructor for _buildCoeff called implicitly.
}

template <class fp_type>
void SparseMatrix<fp_type>::setValue(const std::size_t i, const std::size_t j, fp_type val)
{
    assert(this->_state >= MatrixState::initialized);
    assert(i < this->_nrows);
    assert(j < this->_ncols);

    this->_state = MatrixState::building;

    //Store value
    this->_buildCoeff[std::make_pair(i,j)] = val;
    this->_numnz = this->_buildCoeff.size();

    assert(this->_state == MatrixState::building);
}

template <class fp_type>
fp_type SparseMatrix<fp_type>::getValue(const std::size_t i, const std::size_t j) const noexcept
{
    assert(this->_state > MatrixState::initialized);
    assert(i < this->_nrows);
    assert(j < this->_ncols);
    
    return 0.0;
}

}

// Need to declare the concrete templates within the library for
// use in code that links to libspmv
template class SpMV::SparseMatrix<float>;
template class SpMV::SparseMatrix<double>;
