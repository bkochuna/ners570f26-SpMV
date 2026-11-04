#ifndef __SPMV_SparseMatrix_DEN__
#define __SPMV_SparseMatrix_DEN__

#include "SparseMatrix.hpp"

namespace SpMV
{
    template <class fp_type>
    class SparseMatrix_DEN : public SparseMatrix<fp_type>
    {
        private:
            //_nrows, _ncols, _state, and _numnz are inherited from sparse matrix parent class
            std::vector<fp_type> _values; //store the dense matrix values
        public:
            SparseMatrix_DEN(const size_t nrows, const size_t ncols);
            void assemble();
            MatrixType       getFormat() const noexcept override;
            std::string_view printFormat() const noexcept override;
            //any additional accessors would go here
            //matvec declaration goes here too
    };

    
}

#endif
