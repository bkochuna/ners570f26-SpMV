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
            // Variables for ELL format storage
            size_t max_row_length = 0;
            std::vector<size_t> col_idx;
            std::vector<fp_type> vals;
            
        public:
            SparseMatrix_ELL(const size_t nrows, const size_t ncols);

            // Setters (change information) and getters (grab/return information) for ELL
            void setValue(const size_t i, const size_t j, fp_type val); // Set row index, column index, and value read from Matrix Market
            fp_type getValue(const size_t i, const size_t j);

            // Read-only getters for any matrix inspection
            const size_t& getMaxRowLength() const {
                return max_row_length;
            }
            const std::vector<size_t>& getColIdx() const {
                return col_idx;
            }

    };
}

#endif
