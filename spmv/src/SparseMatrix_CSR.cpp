#include "SparseMatrix_CSR.hpp"

#include <iostream>
#include <stddef.h>

using namespace std;

namespace SpMV
{	
	// TO DO
	template <class fp_type>
    void SparseMatrix_CSR<fp_type>::assemble()
	{
		cout << "Hello from CSR assemble. This has not yet been implemented."

		//This routine needs to convert _buildCoeff into the COO storage format.
	}
	
}

// Need to declare the concrete templates within the library for
// use in code that links to libspmv
template class SpMV::SparseMatrix_CSR<float>;
template class SpMV::SparseMatrix_CSR<double>;
