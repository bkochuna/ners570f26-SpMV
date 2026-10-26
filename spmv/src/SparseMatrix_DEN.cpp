#include "SparseMatrix_DEN.hpp"

#include <iostream>
#include <stddef.h>

#include <vector> 
#include <cassert>

using namespace std;

namespace SpMV {
    template <class fp_type>
    void SparseMatrix_DEN<fp_type>::matvec() {

			return; 
		}

}

// Need to declare the concrete templates within the library for
// use in code that links to libspmv
template class SpMV::SparseMatrix_DEN<float>;
template class SpMV::SparseMatrix_DEN<double>;
