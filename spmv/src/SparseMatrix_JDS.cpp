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
        assert(this->_state == MatrixState::building);
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
}

// Need to declare the concrete templates within the library for
// use in code that links to libspmv
template class SpMV::SparseMatrix_JDS<float>;
template class SpMV::SparseMatrix_JDS<double>;

