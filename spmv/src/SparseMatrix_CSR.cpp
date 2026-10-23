#include "SparseMatrix_COO.hpp"

#include <iostream>
#include <stddef.h>

using namespace std;

namespace SpMV
{	
	// TO DO
	cout << "Hello from CSR assemble. This has not yet been implemented."
}

// Need to declare the concrete templates within the library for
// use in code that links to libspmv
template class SpMV::SparseMatrix_CSR<float>;
template class SpMV::SparseMatrix_CSR<double>;
