#ifndef __SPMV_SparseMatrix_CSR__
#define __SPMV_SparseMatrix_CSR__

#include "SparseMatrix.hpp"

namespace SpMV
{
    template <class fp_type>
    class SparseMatrix_CSR : public SparseMatrix<fp_type>
    {
        private:
            size_t* _ia;            // Row pointer array
            size_t* _ja;            // Column index array
            fp_type* _aij;          // Values

        public:
            SparseMatrix_CSR(const size_t nrows, const size_t ncols);
            void assemble();
            void disassemble()

            MatrixType       getFormat() const noexcept override;
            std::string_view printFormat() const noexcept override;
    };
}

#endif
