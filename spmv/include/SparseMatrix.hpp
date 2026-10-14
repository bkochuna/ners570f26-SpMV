#ifndef __SPMV_SparseMatrix__
#define __SPMV_SparseMatrix__

#include <stddef.h>

using namespace std;

namespace SpMV
{

    enum MatrixState {undefined, initialized, building, assembled};

    template <class fp_type>
    class SparseMatrix
    {
    private:
        MatrixState _state = MatrixState::undefined;
             size_t _nrows = 0;
             size_t _ncols = 0;
             size_t _numnz = 0;
        

    public:
        SparseMatrix(const size_t nrows, const size_t ncols);
        ~SparseMatrix();

        void   assemble();
        void    setValue(const size_t i, const size_t j, fp_type val);
        fp_type getValue(const size_t i, const size_t j);
    };
}

#endif