#include "SparseMatrix_JDS.hpp"
#include <iostream>
#include <stddef.h>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cassert>
#include <stdexcept>

using namespace std;
namespace SpMV
{    
     // Constructs SparseMatrix_JDS with default constructor; Only calls base-class (SparseMatrix) constructor
     // Inputs: nrows -> number of rows in matrix, ncols -> number of columns in matrix
    template <class fp_type>
    SparseMatrix_JDS<fp_type>::SparseMatrix_JDS(const size_t nrows, const size_t ncols) : 
    SparseMatrix<fp_type>::SparseMatrix(nrows, ncols)
    {
        //cout << "Hello from JDS Constructor" << endl; uncomment if debugging
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
        this->_row_srt_idx.resize(this->_nrows);
        iota(this->_row_srt_idx.begin(), this->_row_srt_idx.end(), 0);
        sort(this->_row_srt_idx.begin(), this->_row_srt_idx.end(), [&](size_t a, size_t b)
             { return row_counts[a] > row_counts[b]; });
        _max_row_size = (this->_nrows > 0) ? row_counts[this->_row_srt_idx[0]] : 0;

        // Now initialize the memory for the working member arrays
        _iter_ptr.assign(_max_row_size + 1, 0);
        _row_map.resize(this->_nrows);
        _col_idx.resize(this->_numnz);
        _value.resize(this->_numnz);

        // Load the map now
        for (size_t i = 0; i < this->_nrows; i++)
            _row_map[this->_row_srt_idx[i]] = i;

        // Load iteration pointer with column size
        for (size_t i = 0; i < this->_nrows; i++)
            for (size_t j = 0; j < row_counts[i]; j++)
                _iter_ptr[j + 1]++;

        // Convert the column sizes to a prefix sum
        for (size_t i = 1; i <= _max_row_size; i++)
            _iter_ptr[i] += _iter_ptr[i - 1];

        // Perform the main load now
        for (size_t i = 0; i < this->_numnz; i++)
        {
            size_t new_row = _row_map[temp_row_idx[i]];
            size_t new_offset = _iter_ptr[col_tracker[new_row]] + new_row;
            col_tracker[new_row]++;
            _col_idx[new_offset] = temp_col_idx[i];
            _value[new_offset] = temp_val_arr[i];
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

    for (size_t i = 0; i < _max_row_size; i++)
    {
        for (size_t j = _iter_ptr[i]; j < _iter_ptr[i + 1]; j++)
        {
          // The row is the position in the column (j - prefix start),
          // We then map back to the original row
            size_t orig_row = this->_row_srt_idx[j - _iter_ptr[i]];
            size_t orig_col = _col_idx[j];
            fp_type val = _value[j];
            this->_buildCoeff[{orig_row, orig_col}] = val;
        }
    }
    _row_map.clear();
    _row_srt_idx.clear();
    _col_idx.clear();
    _iter_ptr.clear();
    _value.clear();

    this->_state = MatrixState::building;
    }
// Display the matrix information without modifying it.
// Unassembled matrices raise an error before anything is written to out.
    template <class fp_type>
    void SparseMatrix_JDS<fp_type>::view(std::ostream& out) const
    {
        // Require assembled storage in both debug and release builds.
        if (this->_state != MatrixState::assembled)
        {
            throw std::logic_error("JDS view requires an assembled matrix.");
        }
        // Print the matrix metadata before the JDS storage arrays.
        out << "JDS matrix\n"
            << "Dimensions: " << this->_nrows << " x " << this->_ncols << '\n'
            << "Nonzeros: " << this->_numnz << '\n'
            << "State: assembled\n";

        // Map each sorted storage row back to its original matrix row.
        out << "row_srt_idx (sorted to original):";
        for (const auto index : this->_row_srt_idx)
            out << ' ' << index;
        out << '\n';

    // Mark the boundaries of the jagged diagonals in the flattened arrays.
    out << "iter_ptr: (jagged diagonal offsets)";
    for (const auto index : this->_iter_ptr)
        out << ' ' << index;
    out << '\n';

    // Give the original column index of each stored matrix value.
    out << "col_idx: (column indices of stored values)";
    for (const auto index : this->_col_idx)
        out << ' ' << index;
    out << '\n';

    // List the matrix values in jagged diagonal storage order.
    out << "value: (nonzero values in jagged diagonal order)";
    for (const auto entry : this->_value)
        out << ' ' << entry;
    out << '\n';
}
    template <class fp_type>
    // Function to get the format of the storage type 
    MatrixType SparseMatrix_JDS<fp_type>::getFormat() const noexcept
    {
        return MatrixType::JDS;
    }

    template <class fp_type>
    // Function to print the format of the storage type as a string
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

        // If the matrix is not assembled, search for the entry in _buildCoeff
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
        const size_t sorted_row = this->_row_map[i];
        // Search through each jagged diagonal to find requested entry
        for (size_t k=0; k < this->_max_row_size; ++k)
        {
            // Calculate number of entries stored in diagonal k
            const size_t nrows_chunk = this->_iter_ptr[k+1] - this->_iter_ptr[k];

            // If the sorted row index is greater than or equal to the number of rows in this diagonal, continue to the next diagonal
            if (sorted_row >= nrows_chunk)
            {
                break;
            }
            // Calculate the index in the flattened JDS arrays for the requested entry
            const size_t idx = this->_iter_ptr[k] + sorted_row;
            if (this->_col_idx[idx] == j)
            {
                return this->_value[idx];
            }
        }

        // If the entry does not exist, return 0
        return fp_type{0};
    }

    // Setter for the JDS matrix. Takes in inputs and stores them. - Steven
    template <class fp_type> void SparseMatrix_JDS<fp_type>::setValue(const size_t i, const size_t j, fp_type val)
    {
        // Makes sure the requested row and column are within the bounds of the matrix
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
    // This function takes an input vector with a length equal to the number of columns
    // of this matrix and multiplies it with this matrix in JDS format, returning the
    // resulting output vector.
    // Parameters: x (vector<fp_type>)
    // Returns: y (vector<fp_type>)
    template <class fp_type>
    vector<fp_type> SparseMatrix_JDS<fp_type>::matvec(const vector<fp_type> x)
    {
        // make sure our matrix is actually ready to go
        assert(this->_state == MatrixState::assembled);
        // make sure our input vector is the correct size
        // i.e. its length = the number of matrix columns
        assert(x.size() == this->_ncols);
        cout << "Matrix-vector multiplication starting..." << endl;

        // row and column indices, 
        // starting index of largest row, 
        // number of rows in chunk of nonzeros
        size_t i, j, l_idx, nrows_chunk;
        // output vector - initialize to be all zeros
        // in case a row of the matrix turns out to be all zeros
        // and thus gets skipped over during the matvec routine
        vector<fp_type> y(this->_nrows, 0);
        
        // loop through _iter_ptr array; _max_row_size may be zero, but that's okay
        for (size_t k = 0; k < this->_max_row_size; k++)
        {
            // get starting index of largest row
            l_idx = this->_iter_ptr[k];
            // get number of rows before next column
            nrows_chunk = this->_iter_ptr[k+1] - this->_iter_ptr[k];
            
            // iterate over rows in this chunk of the array
            for (size_t t = 0; t < nrows_chunk; t++)
            {
                // get row index
                i = this->_row_srt_idx[t];
                // get column index
                j = this->_col_idx[l_idx+t];
                // do the multiplication
                y[i] = y[i] + this->_value[l_idx+t]*x[j];
            }
        }     

        cout << "Done!" << endl;
        return y;
    }

}

// Need to declare the concrete templates within the library for
// use in code that links to libspmv
template class SpMV::SparseMatrix_JDS<float>;
template class SpMV::SparseMatrix_JDS<double>;

