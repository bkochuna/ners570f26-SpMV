#include "SparseMatrix_CSR.hpp"

#include <iostream>
#include <stddef.h>
#include <vector>
#include <cassert>
#include <fstream>

using namespace std;

namespace SpMV
{	
	template <class fp_type>
	// added lines 
	SparseMatrix_CSR<fp_type>::SparseMatrix_CSR(const size_t nrows, const size_t ncols) :
		SparseMatrix<fp_type>(nrows, ncols)
	{
	}

	template <class fp_type>
	MatrixType SparseMatrix_CSR<fp_type>::getFormat() const noexcept
	{
		return MatrixType::CSR;
	}

	// Getters:
	// .data() gives point o vector's array
	// asserts only when assembled
	// Names _ia, _ja, and _aij match to defined CSR Matrix class
	
	template <class fp_type>
	const size_t* SparseMatrix_CSR<fp_type>::getRowPtr() const noexcept
	{
		assert(this->_state == MatrixState::assembled);
		return this->_ia.data();
	}

	template <class fp_type>
	const size_t* SparseMatrix_CSR<fp_type>::getColIdx() const noexcept
	{
		assert(this->_state == MatrixState::assembled);
		return this->_ja.data();
	}

	template <class fp_type>
	const fp_type* SparseMatrix_CSR<fp_type>::getValues() const noexcept
	{
		assert(this->_state == MatrixState::assembled);
		return this->_aij.data();
	}

	template <class fp_type>
	std::string_view SparseMatrix_CSR<fp_type>::printFormat() const noexcept
	{
		return "CSR";
	}

	template <class fp_type>
	// end of added lines
    
	void SparseMatrix_CSR<fp_type>::assemble()
	{
		// This routine converts _buildCoeff into the CSR storage format.
		assert(this->_state == MatrixState::building);
		cout << "Hello from CSR assemble.";

		// Useful attributes
		size_t N = this->_nrows;
		size_t nnz = this->_numnz;

		// Temporary vectors
		vector<size_t> temp_row_idx(nnz);
		vector<size_t> temp_col_idx(nnz);
		vector<fp_type> temp_val_arr(nnz);
		vector<size_t> temp(N);
		
		// Stored vectors
		this->_ia = vector<size_t>(N+1);
		this->_ja = vector<size_t>(nnz);
		this->_aij = vector<fp_type>(nnz);

		// Populate temporary vectors in COO format
		size_t i = 0;
		for (auto& [key, val] : this->_buildCoeff) {
			temp_row_idx[i] = key.first;
			temp_col_idx[i] = key.second;
			temp_val_arr[i] = val;
			i++;
		}

		// Convert COO storage to CSR vectors. First, find number 
		// of entries with each row index
		for (size_t k = 0; k < nnz; k++) {
			this->_ia[temp_row_idx[k] + 1]++;
		}

		// Then cumulatively sum row pointer array
		for (size_t k = 0; k < N; k++) {
			this->_ia[k + 1] += this->_ia[k];
		}

		// Sort entries into col and val array
		for (size_t k = 0; k < N; k++) {
			temp[k] = this->_ia[k];
		}

		for (size_t k = 0; k < nnz; k++) {
			size_t r = temp_row_idx[k];
			size_t dest = temp[r];
			temp[r]++;

			this->_ja[dest] = temp_col_idx[k];
			this->_aij[dest] = temp_val_arr[k];
		}

		this->_state = MatrixState::assembled;
	}

	template <class fp_type>
	void SparseMatrix_CSR<fp_type>::disassemble()
	{
		assert(this->_state == MatrixState::assembled);

		size_t N = this->_nrows;

		// Clear old data
		this->_buildCoeff.clear();

		// Build _buildCoeff from array data
		for (size_t r = 0; r < N; r++) {
			for (size_t k = _ia[r]; k < _ia[r+1]; k++) {
				size_t c = _ja[k];
				fp_type val = _aij[k]; 

				this->_buildCoeff[{r, c}] = val;
			}
		}

		this->_ia.clear();
		this->_ja.clear();
		this->_aij.clear();

		this->_state = MatrixState::building;
	}

	// Getter for A(i,j)
	template <class fp_type>
	fp_type SparseMatrix_CSR<fp_type>::getValue(const size_t i, const size_t j) const noexcept
	{
		assert(this->_state > MatrixState::initialized);
		assert(i < this->_nrows);
		assert(j < this->_ncols);

		// Building: values live in the _buildCoeff map (unset entries are zero)
		if (this->_state == MatrixState::building) {
			const auto it = this->_buildCoeff.find(std::make_pair(i, j));
			return (it != this->_buildCoeff.end()) ? it->second : static_cast<fp_type>(0);
		}

		// Assembled: row i entries are stored at positions _ia[i] .. _ia[i+1]-1
		for (size_t k = this->_ia[i]; k < this->_ia[i + 1]; ++k) {
			if (this->_ja[k] == j) {
				return this->_aij[k];
			}
		}

		// (i,j) is not stored, so it is a zero of the sparse matrix
		return static_cast<fp_type>(0);
	}


	template <class fp_type>
	void SparseMatrix_CSR<fp_type>::view(std::ostream& out) const
	{
		assert(this->_state == MatrixState::assembled);

		out << "CSR Matrix (" << this->_nrows << " x "
			<< this->_ncols << "), nnz = "
			<< this->_aij.size() << endl;

		out << "Row  Col  Value" << endl;

		for (size_t i = 0; i < this->_nrows; ++i) {
			for (size_t k = this->_ia[i]; k < this->_ia[i + 1]; ++k) {
				out << i << "    "
					<< this->_ja[k] << "    "
					<< this->_aij[k] << endl;
			}
		}
	}

	template <class fp_type>
	bool SparseMatrix_CSR<fp_type>::view(const std::string& filename) const
	{
		std::ofstream file(filename);
		if (!file.is_open()) {
			return false; // bad path, no permission, etc.
		}
		view(file);
		return true;
	}

    template <class fp_type>
    void SparseMatrix_CSR<fp_type>::matvec(size_t nmults, 
                                            vector<fp_type>& vec) const
    {
        assert(this->_state == MatrixState::assembled);
        assert(vec.size() == this->_nrows);

        vector<fp_type> in { vec };
        vector<fp_type> out ( this->_nrows );

        for (size_t n = 0; n < nmults; n++) {
            for (size_t i = 0; i < this->_nrows; i++) {
                fp_type sum { 0 };
                for (size_t c = _ia[i]; c < _ia[i + 1]; c++) {
                    sum += _aij[c] * in[_ja[c]];
                }
                out[i] = sum;
            }
            // swaps
            swap(in, out);
        }
        vec = move(in);
    }
    
}

// Need to declare the concrete templates within the library for
// use in code that links to libspmv
template class SpMV::SparseMatrix_CSR<float>;
template class SpMV::SparseMatrix_CSR<double>;
