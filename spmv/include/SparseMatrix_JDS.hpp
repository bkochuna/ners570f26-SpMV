#ifndef __SPMV_SparseMatrix_JDS__
#define __SPMV_SparseMatrix_JDS__

#include "SparseMatrix.hpp"
#include <vector>

namespace SpMV
{
    template <class fp_type>
    class SparseMatrix_JDS : public SparseMatrix<fp_type>
    {
    public:
        SparseMatrix_JDS(const size_t nrows, const size_t ncols);
        void assemble();
        void disassemble(); // added for disassembly

        MatrixType getFormat() const noexcept override;
        std::string_view printFormat() const noexcept override;

    protected:
        size_t max_row_size; // size of row with maximum number of non-zeros in matrix
        vector<int> row_map; 
        vector<int> row_srt_idx; // row indices sorted
        vector<int> col_idx; // column index of each entry in value array
        vector<int> iter_ptr; // array of indices for col_idx corresponding to each entry from the longest non-zero row of the matrix 
        vector<fp_type> value; //matrix values
    };
}
#endif

