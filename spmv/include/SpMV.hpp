#ifndef __SPMV__
#define __SPMV__

#include <iostream>

namespace SpMV
{
    class SparseMatrix
    {
    private:
        /* data */
    public:
        SparseMatrix(/* args */);
        ~SparseMatrix();
    };

    SparseMatrix::SparseMatrix(/* args */)
    {
        std::cout << "Hello from SparseMatrix Constructor!" << std::endl;
    }

    SparseMatrix::~SparseMatrix()
    {
        std::cout << "Hello from SparseMatrix Destructor!" << std::endl;
    }
}

#endif
