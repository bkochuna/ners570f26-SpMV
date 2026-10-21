#ifndef __SPMV_SparseMatrix_ELL__
#define __SPMV_SparseMatrix_ELL__

#include "SparseMatrix.hpp"

namespace SpMV
{
    template <class fp_type>
    class SparseMatrix_ELL : public SparseMatrix<fp_type>
    {
        // Private variables for ELL
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

            // Setters (change information) and getters (grab/return information) for ELL
            size_t getMaxRowLength() const {
                return max_row_length;
            }
            void setMaxRowLength(size_t newRowLength) {
                max_row_length = newRowLength;
            }

            //const std::vector<double> getValues() const { // Similar one in SparseMatrix... maybe just call that?
            //    return vals;
            //}
            fp_type getValue(const size_t i, const size_t j,); // Get value from SparseMatrix
            const std::vector<size_t> getColumnIdx(const size_t j);

    };
}

#endif
