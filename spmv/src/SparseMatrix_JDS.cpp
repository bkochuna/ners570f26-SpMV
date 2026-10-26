#include "SparseMatrix_JDS.hpp"

#include <iostream>
#include <stddef.h>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cassert>

using namespace std;
namespace SpMV
{
    template <class fp_type>
    SparseMatrix_JDS<fp_type>::SparseMatrix_JDS(const size_t nrows, const size_t ncols) : 
    SparseMatrix<fp_type>::SparseMatrix(nrows, ncols)
    {
        cout << "Hello from JDS Constructor" << endl;
    }

    template <class fp_type>
    void SparseMatrix_JDS<fp_type>::assemble()
    {
        // This routine converts _buildCoeff into the JDS storage format.
        assert(this->_state <= MatrixState::building);
        cout << "Hello from JDS assemble" << endl;

        this->_numnz = this->_buildCoeff.size();
        // Temporary working vectors
        vector<size_t> row_counts(this->_nrows);
        vector<size_t> col_tracker(this->_nrows);
        vector<size_t>
            temp_row_idx(this->_numnz);
        vector<size_t> temp_col_idx(this->_numnz);
        vector<fp_type> temp_val_arr(this->_numnz);

        // ===== First convert into COO and count row sizes =====
        size_t entry_idx = 0;
        for (auto &[key, val] : this->_buildCoeff)
        {
            row_counts[key.first]++;
            temp_row_idx[entry_idx] = key.first;
            temp_col_idx[entry_idx] = key.second;
            temp_val_arr[entry_idx] = val;
            entry_idx++;
        }

        // ===== Now convert from COO to JDS =====
        // Sort with an index array by row size
        this->row_srt_idx.resize(this->_nrows);
        iota(this->row_srt_idx.begin(), this->row_srt_idx.end(), 0);
        sort(this->row_srt_idx.begin(), this->row_srt_idx.end(), [&](size_t a, size_t b)
             { return row_counts[a] > row_counts[b]; });
        max_row_size = (this->_nrows > 0) ? row_counts[this->row_srt_idx[0]] : 0;

        // Now initialize the memory for the working member arrays
        iter_ptr.assign(max_row_size + 1, 0);
        row_map.resize(this->_nrows);
        col_idx.resize(this->_numnz);
        value.resize(this->_numnz);

        // Load the map now
        for (size_t i = 0; i < this->_nrows; i++)
            row_map[this->row_srt_idx[i]] = i;

        // Load iteration pointer with column size
        for (size_t i = 0; i < this->_nrows; i++)
            for (size_t j = 0; j < row_counts[i]; j++)
                iter_ptr[j + 1]++;

        // Convert the column sizes to a prefix sum
        for (size_t i = 1; i <= max_row_size; i++)
            iter_ptr[i] += iter_ptr[i - 1];

        // Perform the main load now
        for (size_t i = 0; i < this->_numnz; i++)
        {
            size_t new_row = row_map[temp_row_idx[i]];
            size_t new_offset = iter_ptr[col_tracker[new_row]] + new_row;
            col_tracker[new_row]++;
            col_idx[new_offset] = temp_col_idx[i];
            value[new_offset] = temp_val_arr[i];
        }

        this->_buildCoeff.clear();
        this->_state = MatrixState::assembled;
    }

    template <class fp_type>
    void SparseMatrix_JDS<fp_type>::disassemble()
    {
        cout << "Hello from JDS disassemble" << endl;

    assert(this->_state == MatrixState::assembled);

    this->_buildCoeff.clear();

    for (size_t i = 0; i < max_row_size; i++)
    {
        for (size_t j = iter_ptr[i]; j < iter_ptr[i + 1]; j++)
        {
            size_t orig_row = this->row_srt_idx[j - iter_ptr[i]];
            size_t orig_col = col_idx[j];
            fp_type val = value[j];
            this->_buildCoeff[{orig_row, orig_col}] = val;
        }
    }
    
    row_map.clear();
    row_srt_idx.clear();
    col_idx.clear();
    iter_ptr.clear();
    value.clear();

    this->_state = MatrixState::building;
    }

    template <class fp_type>
    MatrixType SparseMatrix_JDS<fp_type>::getFormat() const noexcept
    {
        return MatrixType::JDS;
    }

    template <class fp_type>
    std::string_view SparseMatrix_JDS<fp_type>::printFormat() const noexcept
    {
        return "JDS";
    }

    // Getter for the JDS matrix. Takes in inputs and returns the value at that location. - Steven
    template <class fp_type>
    fp_type SparseMatrix_JDS<fp_type>::getValue(const size_t i, const size_t j) const noexcept
    {
        // Makes sure the requested row and column are within the bounds of the matrix
        assert(i < this->_nrows);
        assert(j < this->_ncols);

        // If the matrix is not assembled, we can just use the base class's getValue method
        if (this->_state != MatrixState::assembled)
        {
            auto it = this->_buildCoeff.find(std::make_pair(i, j));
            // If the entry is found, return its value; otherwise, return 0
            if (it != this->_buildCoeff.end())
            {
                return it->second;
            }
            return fp_type{0};
        }
        
        // If the matrix is assembled, convert the original row index to the sorted row position in JDS storage
        const size_t sorted_row = this->row_map[i];
        // Search through each jagged diagonal to find requested entry
        for (size_t k=0; k < this->max_row_size; ++k)
        {
            // Calculate number of entries stored in diagonal k
            const size_t nrows_chunk = this->iter_ptr[k+1] - this->iter_ptr[k];

            // If the sorted row index is greater than or equal to the number of rows in this diagonal, continue to the next diagonal
            if (sorted_row >= nrows_chunk)
            {
                continue;
            }
            // Calculate the index in the flattened JDS arrays for the requested entry
            const size_t idx = this->iter_ptr[k] + sorted_row;
            if (this->col_idx[idx] == j)
            {
                return this->value[idx];
            }
        }

        // If the entry does not exist, return 0
        return fp_type{0};
    }

    // Setter for the JDS matrix. Takes in inputs and stores them. - Steven
    template <class fp_type> void SparseMatrix_JDS<fp_type>::setValue(const size_t i, const size_t j, fp_type val)
    {
        assert(i < this->_nrows);
        assert(j < this->_ncols);

        // Makes sure the matrix is initialized before setting a value
        assert(this->_state >= MatrixState::initialized);

        // If the matrix is assembled, disassemble it first
        if (this->_state == MatrixState::assembled)
        {
            this->disassemble();
        }

        // Use the base class setter to update the entry in _buildCoeff
        SparseMatrix<fp_type>::setValue(i, j, val);

        
    }

    // matrix-vector multiplication - Hanga
    template <class fp_type>
    vector<fp_type> SparseMatrix_JDS<fp_type>::matvec(vector<fp_type> vecin, size_t nmults) // NOTE: default is nmults=1 in header file
    {
        // make sure our matrix is actually ready to go
        assert(this->_state == MatrixState::assembled);
        cout << "Matrix-vector multiplication starting..." << endl;

        // row and column indices, 
        // starting index of largest row, 
        // number of rows in chunk of nonzeros
        size_t i, j, l_idx, nrows_chunk;
        // output vector - initialize to be all zeros
        // in case a row of the matrix turns out to be all zeros
        // and thus gets skipped over during the matvec routine
        vector<fp_type> vecout(this->_nrows, 0);

        // local temporary copy of vecin that can be overwritten if nmults>1
        vector<fp_type> vecin_temp = vecin;
        
        // loop through desired number of multiplications
        for (size_t n = 0; n < nmults; n++)
        {
            // loop through iter_ptr array; max_row_size may be zero, but that's okay
            for (size_t k = 0; k < this->max_row_size; k++)
            {
                // get starting index of largest row
                l_idx = this->iter_ptr[k];
                // get number of rows before next column
                nrows_chunk = this->iter_ptr[k+1] - this->iter_ptr[k];
                
                // iterate over rows in this chunk of the array
                for (size_t t = 0; t < nrows_chunk; t++)
                {
                    // get row index
                    i = this->row_srt_idx[t];
                    // get column index
                    j = this->col_idx[l_idx+t];
                    // do the multiplication
                    vecout[i] = vecout[i] + this->value[l_idx+t]*vecin_temp[j];
                }
            }
            // if we're not on the last iteration
            if (n < nmults-1)
            {
                // make last output vector the new input vector
                vecin_temp = vecout;
                // and zero out the output vector
                fill(vecout.begin(), vecout.end(), 0.0);
            }        
        }

        cout << "Done!" << endl;
        return vecout;
    }

}

// Need to declare the concrete templates within the library for
// use in code that links to libspmv
template class SpMV::SparseMatrix_JDS<float>;
template class SpMV::SparseMatrix_JDS<double>;

