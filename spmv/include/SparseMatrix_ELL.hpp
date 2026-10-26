#ifndef __SPMV_SparseMatrix_ELL__
#define __SPMV_SparseMatrix_ELL__

#include "SparseMatrix.hpp"

namespace SpMV
{
    template <class fp_type>
    class SparseMatrix_ELL : public SparseMatrix<fp_type>
    {
        public:
            SparseMatrix_ELL(const size_t nrows, const size_t ncols);
            void assemble();

            MatrixType       getFormat() const noexcept override;
            std::string_view printFormat() const noexcept override;

            void view();
    };
}

std::vetcor<double> matvec{
    const SparseMatrix_ELL& A, 
    const std::vector<double>& x
};
   

#endif
