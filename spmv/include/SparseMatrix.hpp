#ifndef __SPMV_SparseMatrix__
#define __SPMV_SparseMatrix__

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
        SparseMatrix(const size_t nrows, const size_t ncols);
        ~SparseMatrix();
    };

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

#endif