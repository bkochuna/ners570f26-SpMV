#include "SparseMatrix_DEN.hpp"

#include <iostream>
#include <stddef.h>

#include <vector> 
#include <cassert>

using namespace std;

namespace SpMV {
	template <class fp_type>
		void SparseMatrix_DEN<fp_type>::matvec(vector<fp_type> inputVec, vector<fp_type> outputVec) {
			int i, j; 
			fp_type acc; // accumulator for our multiplication

			for(i = 0; i < this->_nrows; i++){
				acc = 0.0;

				for(j = 0; j < this->_ncols; j++){
					acc += this->_a[i][j] * inputVec[j];
				}

				outputVec[i] = acc; // write accumulator to our output vector 
			}

			return; 
		}

}

// Need to declare the concrete templates within the library for
// use in code that links to libspmv
template class SpMV::SparseMatrix_DEN<float>;
template class SpMV::SparseMatrix_DEN<double>;
