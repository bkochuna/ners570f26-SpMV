#ifndef __SPMV_SparseMatrix_CSR__
#define __SPMV_SparseMatrix_CSR__

#include "SparseMatrix.hpp"

#include <cstddef> // added these
#include <vector>

namespace SpMV
{
    template <class fp_type>
    class SparseMatrix_CSR : public SparseMatrix<fp_type>
    {
        private:
            std::vector<std::size_t> _ia; // Row offsets: nrows + 1 entries after assembly
            std::vector<std::size_t> _ja; // Column indices: one per stored value
            std::vector<fp_type> _aij;    // Stored matrix values

        public:
            SparseMatrix_CSR(const size_t nrows, const size_t ncols);
            void assemble() override;
            void disassemble();
            void view() const;

            MatrixType       getFormat() const noexcept override;
            std::string_view printFormat() const noexcept override;
    };
}

#endif
