#ifndef __SPMV_SparseMatrix_COO__
#define __SPMV_SparseMatrix_COO__

#include "SparseMatrix.hpp"
#include <vector>

namespace SpMV
{
    template <class fp_type>
    class SparseMatrix_COO : public SparseMatrix<fp_type>
    {
        private:
            size_t*  _ia   = nullptr;
            size_t*  _ja   = nullptr;
            fp_type* _vals = nullptr;

        public:
            SparseMatrix_COO(const size_t nrows, const size_t ncols);
            SparseMatrix_COO(const size_t nnz, const size_t* ia, const size_t* ja, const fp_type* vals);
            SparseMatrix_COO(const vector<size_t>& ia, const vector<size_t>& ja, const vector<fp_type>& vals);
            void assemble();

            MatrixType       getFormat()   const noexcept override;
            std::string_view printFormat() const noexcept override;
    };
}

#endif
