#ifndef __SPMV_SparseMatrix_JDS__
#define __SPMV_SparseMatrix_JDS__

#include "SparseMatrix.hpp"
#include <cstddef>
#include <vector>
#include <iostream>

namespace SpMV
{
    template <class fp_type>
    class SparseMatrix_JDS : public SparseMatrix<fp_type>
    {
    public:
        SparseMatrix_JDS(const size_t nrows, const size_t ncols);
        void assemble(); // Assembly: takes the matrix in its original format and assembles it into the JDS storage format
        void disassemble(); // Disassembly: takes the matrix in the JDS storage format and returns it back to original format
        void view(std::ostream& out = std::cout) const; // View: displays matrix in the JDS storage format arrays along with some other matrix attributes (dimensions,number of nonzeros)

        MatrixType getFormat() const noexcept override; // Get Format: Returns the matrix storage type we are using
        std::string_view printFormat() const noexcept override; // Print format: prints the matrix storage type format as a string

        void setValue(const size_t i, const size_t j, fp_type val); // Set Value: Setter for the JDS matrix. Takes in indices and value as inputs and stores them in matrix.
        
        [[nodiscard]] // Warning if the return value is not used
        fp_type getValue(const size_t i, const size_t j) const noexcept; // Get Value: Getter for the JDS matrix. Takes in inputs and returns the value at that location.

        vector<fp_type> matvec(const vector<fp_type> x); // Matrix-_Vector Multiplication: Performs matvec given some input vector 

    private:
        size_t _max_row_size; // size of row with maximum number of non-zeros in matrix
        vector<size_t> _row_map; // Row indices original -> sorted; the value at each index is the position that the original row of that index is at after being sorted (ordered by decreasing nonzeros)
        vector<size_t> _row_srt_idx; // Row indices sorted -> original; tells you which original row is in each position (ordered by decreasing nonzeros)
        vector<size_t> _col_idx; // column index of each entry in value array
        vector<size_t> _iter_ptr; // array of indices for col_idx corresponding to each entry from the longest non-zero row of the matrix 
        vector<fp_type> _value; //matrix values
    };
}
#endif

