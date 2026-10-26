#include "SparseMatrix_CSR.hpp"

#include <iostream>
#include <stddef.h>
#include <vector>
#include <cassert>

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


	template <class fp_type>
	void SparseMatrix_CSR<fp_type>::view() const
	{
		assert(this->_state == MatrixState::assembled);

		cout << "CSR Matrix (" << this->_nrows << " x "
			<< this->_ncols << "), nnz = "
			<< this->_aij.size() << endl;

		cout << "Row  Col  Value" << endl;

		for (size_t i = 0; i < this->_nrows; ++i) {
			for (size_t k = this->_ia[i]; k < this->_ia[i + 1]; ++k) {
				cout << i << "    "
					<< this->_ja[k] << "    "
					<< this->_aij[k] << endl;
			}
		}
	}

}

// Need to declare the concrete templates within the library for
// use in code that links to libspmv
template class SpMV::SparseMatrix_CSR<float>;
template class SpMV::SparseMatrix_CSR<double>;
