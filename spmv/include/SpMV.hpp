#ifndef __SPMV__
#define __SPMV__

#include <iostream>
#include <stddef.h>

using namespace std;

namespace SpMV
{
    class SparseMatrix
    {
    private:
        size_t _nrows = 0;
        size_t _ncols = 0;
        size_t _numnz = 0;

    public:
        SparseMatrix(int nrows, int ncols);
        ~SparseMatrix();
    };

    SparseMatrix::SparseMatrix(int nrows, int ncols) :
        _nrows(nrows), _ncols(ncols)
    {
        cout << "Hello from SparseMatrix Constructor!" << endl;
        this->_nrows = nrows;
        this->_ncols = ncols;
    }

    SparseMatrix::~SparseMatrix()
    {
        cout << "Hello from SparseMatrix Destructor!" << endl;
        cout << "this->_nrows=" << this->_nrows << endl;
        cout << "this->_ncols=" << this->_ncols << endl;
        cout << "this->_numnz=" << this->_numnz << endl;
    }
}

#endif
