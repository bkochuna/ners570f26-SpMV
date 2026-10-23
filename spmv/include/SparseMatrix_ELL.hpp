#ifndef __SPMV_SparseMatrix_ELL__
#define __SPMV_SparseMatrix_ELL__

#include "SparseMatrix.hpp"
// #include "SparseMatrix_CSR.hpp"

namespace SpMV
{
    template <class fp_type>
    class SparseMatrix_ELL : public SparseMatrix<fp_type>
    {
        // Private variables for ELL
        private:
            // Variables for ELL format storage
            size_t _maxrowlength = 0;
            std::vector<size_t> _colidx;
            std::vector<fp_type> _vals;
            
        public:
            SparseMatrix_ELL(const size_t nrows, const size_t ncols);

            // Setters (change information) and getters (grab/return information) for ELL
            void setValue(const size_t i, const size_t j, fp_type val); // Set row index, column index, and value read from Matrix Market

            // Getters for any matrix inspection
            void setMaxRowLength(size_t length) {
                _maxrowlength = length;
            }
            size_t getMaxRowLength() const {
                return max_row_length;
            }
            size_t getColIdx(const size_t i, const size_t slot, const size_t totalRows = 0) const;
            fp_type getValue(const size_t i, const size_t slot, const size_t totalRows = 0) const;

            // From original code template
            MatrixType       getFormat() const noexcept override;
            std::string_view printFormat() const noexcept override;

    };
}

#endif
