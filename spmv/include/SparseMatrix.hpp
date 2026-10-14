#ifndef __SPMV_SparseMatrix__
#define __SPMV_SparseMatrix__

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

        void   setValue(const size_t i, const size_t j, double val);
        double getValue(const size_t i, const size_t j);
        void   assemble();
    };
}

#endif