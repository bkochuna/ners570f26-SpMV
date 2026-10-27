#ifndef __SPMV_SparseMatrix_CSR__
#define __SPMV_SparseMatrix_CSR__

#include "SparseMatrix.hpp"

#include <cstddef> // added these
#include <vector>
#include <iostream>
#include <string>

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
			
			// Add view such that it defaults to screen but can be output to a file
			void view(std::ostream& out = std::cout) const;
			[[nodiscard]] bool view(const std::string& filename) const;

			// Accessor returns A(i,j) read from CSR array once assembled.
			[[nodiscard]] fp_type getValue(const size_t i, const size_t j) const noexcept override;
			[[nodiscard]] const size_t* getRowPtr() const noexcept; // _ia, length nrows + 1
			[[nodiscard]] const size_t* getColIdx() const noexcept; // _ja, length numnz
			[[nodiscard]] const fp_type* getValues() const noexcept; // _aij, length numnz

            MatrixType       getFormat() const noexcept override;
            std::string_view printFormat() const noexcept override;

            void matvec(size_t nmults, vector <fp_type>& vec) const;
    };
}

#endif
