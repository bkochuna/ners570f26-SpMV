#ifndef __SPMV__
#define __SPMV__

#include <iostream>

using namespace std;

namespace SpMV
{
    class SparseMatrix
    {
    private:
        int _nrows = 0;
        int _ncols = 0;
        int _numnz = 0;

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
