#ifndef __SPMV__
#define __SPMV__

#include <iostream>

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

    SparseMatrix::SparseMatrix(int nrows, int ncols)
    {
        std::cout << "Hello from SparseMatrix Constructor!" << std::endl;
        this->_nrows = nrows;
        this->_ncols = ncols;
    }

    SparseMatrix::~SparseMatrix()
    {
        std::cout << "Hello from SparseMatrix Destructor!" << std::endl;
        std::cout << "this->_nrows=" << this->_nrows << std::endl;
        std::cout << "this->_ncols=" << this->_ncols << std::endl;
        std::cout << "this->_numnz=" << this->_numnz << std::endl;
    }
}

#endif
