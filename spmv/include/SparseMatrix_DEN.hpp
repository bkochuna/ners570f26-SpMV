#ifndef __SPMV_SparseMatrix_DEN__
#define __SPMV_SparseMatrix_DEN__

#include "SparseMatrix.hpp"

#include <vector> // std::vector

namespace SpMV
{
    template <class fp_type>
    class SparseMatrix_DEN : public SparseMatrix<fp_type>
    {
        public:
            SparseMatrix_DEN(const size_t nrows, const size_t ncols);
            void assemble() override;
            void disassemble();
            void matvec(const std::vector<fp_type>& inputVec, std::vector<fp_type>& outputVec) const;

            std::vector<std::vector<fp_type>> _a;
            MatrixType       getFormat() const noexcept override;
            std::string_view printFormat() const noexcept override;
    };
}

#endif
