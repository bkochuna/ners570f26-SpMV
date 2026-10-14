#include "SparseMatrix.hpp"

#include <iostream>
#include <cassert>

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

void SparseMatrix::setValue(const size_t i, const size_t j, double val)
{
    cout << "i=" << i << endl;
    cout << "j=" << j << endl;
    cout << "val=" << val << endl;
}

double SparseMatrix::getValue(const size_t i, const size_t j)
{
    cout << "i=" << i << endl;
    cout << "j=" << j << endl;
    return 0.0;
}

void SparseMatrix::assemble()
{
    cout << "Hello from assemble()" << endl;
}

}