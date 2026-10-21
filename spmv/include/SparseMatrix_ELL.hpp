#ifndef __SPMV_SparseMatrix_ELL__
#define __SPMV_SparseMatrix_ELL__

#include "SparseMatrix.hpp"

namespace SpMV
{
    template <class fp_type>
    class SparseMatrix_ELL : public SparseMatrix<fp_type>
    {
        private:
            size_t max_row_length = 0;
            std::vector<size_t> col_idx;
            std::vector<double> vals;
            //double * vals;

        public:
            SparseMatrix_ELL(const size_t nrows, const size_t ncols);
            void assemble();

            MatrixType       getFormat() const noexcept override;
            std::string_view printFormat() const noexcept override;
    };
}

#endif
