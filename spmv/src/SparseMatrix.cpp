#include "SparseMatrix.hpp"

#include <iostream>

namespace SpMV
{

SparseMatrix::SparseMatrix(const size_t nrows, const size_t ncols) :
    _nrows(nrows), _ncols(ncols)
{
    cout << "Hello from SparseMatrix Constructor!" << endl;
}

SparseMatrix::~SparseMatrix()
{
    cout << "Hello from SparseMatrix Destructor!" << endl;
    cout << "this->_nrows=" << this->_nrows << endl;
    cout << "this->_ncols=" << this->_ncols << endl;
    cout << "this->_numnz=" << this->_numnz << endl;
}

}