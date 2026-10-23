#include "SparseMatrix_CSR.hpp"

#include <iostream>
#include <stddef.h>
#include <vector>

using namespace std;

namespace SpMV
{	
	template <class fp_type>
    void SparseMatrix_CSR<fp_type>::assemble()
	{
		// This routine converts _buildCoeff into the CSR storage format.
		assert(this->_state == MatrixState::building);
		cout << "Hello from CSR assemble.";

		// Temporary row index array 
		vector<size_t> temp_row_idx(this->_numnz);
		
		this->_ia = vector<size_t>(this->_nrows+1);
		this->_ja = vector<size_t>(this->_numnz);
		this->_aij = vector<fp_type>(this->_numnz);
	}

	template <class fp_type>
	void SparseMatrix_CSR<fp_type>::disassemble()
	{
		assert(this->_state == MatrixState::assembled);

		this->_ia.clear();
		this->_ja.clear();
		this->_aij.clear();
	}
}

// Need to declare the concrete templates within the library for
// use in code that links to libspmv
template class SpMV::SparseMatrix_CSR<float>;
template class SpMV::SparseMatrix_CSR<double>;
