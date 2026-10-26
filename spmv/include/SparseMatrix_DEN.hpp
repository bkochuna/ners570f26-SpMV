#ifndef __SPMV_SparseMatrix_DEN__
#define __SPMV_SparseMatrix_DEN__

#include "SparseMatrix.hpp"

namespace SpMV
{
    template <class fp_type>
    class SparseMatrix_DEN : public SparseMatrix<fp_type>
    {
        public:
            SparseMatrix_DEN(const size_t nrows, const size_t ncols);
            void assemble();

            MatrixType       getFormat() const noexcept override;
            std::string_view printFormat() const noexcept override;
            void view() const;
    };
}

#endif
