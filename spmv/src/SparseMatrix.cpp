#include "SparseMatrix.hpp"

#include <iostream>
#include <cassert>

namespace SpMV
{

SparseMatrix::SparseMatrix(const size_t nrows, const size_t ncols) :
    _nrows(nrows), _ncols(ncols)
{
    assert(this->_state == -1);
    cout << "Hello from SparseMatrix Constructor!" << endl;
    this->_state = 0;
    assert(this->_state == 0);
}

SparseMatrix::~SparseMatrix()
{
    cout << "Hello from SparseMatrix Destructor!" << endl;
    cout << "this->_nrows=" << this->_nrows << endl;
    cout << "this->_ncols=" << this->_ncols << endl;
    cout << "this->_numnz=" << this->_numnz << endl;
    this->_state=-1;
}

void SparseMatrix::setValue(const size_t i, const size_t j, double val)
{
    assert(this->_state >= 0);
    this->_state = 1; // building

    cout << "i=" << i << endl;
    cout << "j=" << j << endl;
    cout << "val=" << val << endl;

    assert(this->_state == 1);
}

double SparseMatrix::getValue(const size_t i, const size_t j)
{
    assert(this->_state > 0);
    cout << "i=" << i << endl;
    cout << "j=" << j << endl;
    return 0.0;
}

void SparseMatrix::assemble()
{
    assert(this->_state > 0);
    cout << "Hello from assemble()" << endl;
    this->_state = 2; // assembled
    assert(this->_state == 2);
}

}