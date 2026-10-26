#include "SparseMatrix_DEN.hpp"

#include <iostream>
#include <stddef.h>
#include <cassert>

using namespace std;

namespace SpMV
{
   template <class fp_type>
   void SparseMatrix_DEN<fp_type>::view() const
   {
    assert(this->_state > MatrixState::initialized);
    for (size_t i = 0; i < this->_nrows; ++i)
    {
        for (size_t j = 0; j < this->_ncols; ++j)
        {
            cout << this->getValue(i,j) << " ";
        }
        cout << endl;
    }

   }
}

// Need to declare the concrete templates within the library for
// use in code that links to libspmv
template class SpMV::SparseMatrix_DEN<float>;
template class SpMV::SparseMatrix_DEN<double>;
