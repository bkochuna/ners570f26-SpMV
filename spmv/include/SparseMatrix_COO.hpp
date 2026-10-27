#ifndef __SPMV_SparseMatrix_COO__
#define __SPMV_SparseMatrix_COO__

#include "SparseMatrix.hpp"
#include <vector>

namespace SpMV
{
    template <class fp_type>
    class SparseMatrix_COO : public SparseMatrix<fp_type>
    {
        private:
            // a better, more idiomatic way of doing this is with uniq_ptr or vector
            size_t*  _ia   = nullptr;
            size_t*  _ja   = nullptr;
            fp_type* _vals = nullptr;

            void _disassemble()  noexcept;

        public:
            SparseMatrix_COO(const size_t nrows, const size_t ncols);

            // Overloaded constructors
            SparseMatrix_COO(const size_t nnz, const size_t* ia, const size_t* ja, const fp_type* vals);
            SparseMatrix_COO(const std::vector<size_t>& ia, const std::vector<size_t>& ja, const std::vector<fp_type>& vals);

            //Copy and Move constructors
            SparseMatrix_COO(const SparseMatrix_COO& other);                 //Copy
            SparseMatrix_COO& operator=(const SparseMatrix_COO& other);      //Copy assignment
            SparseMatrix_COO(SparseMatrix_COO&& other) noexcept;             //Move
            SparseMatrix_COO& operator=(SparseMatrix_COO&& other) noexcept;  //Move assginment
            
            ~SparseMatrix_COO(); // Destructor
            void assemble();

            MatrixType       getFormat()   const noexcept override;
            std::string_view printFormat() const noexcept override;
    };
}

#endif
