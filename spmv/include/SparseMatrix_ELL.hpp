#ifndef __SPMV_SparseMatrix_ELL__
#define __SPMV_SparseMatrix_ELL__

#include <vector>
#include <cstddef>
#include <string_view>
#include <iosfwd>
#include "SparseMatrix.hpp"

namespace SpMV
{
    template <class fp_type>
    class SparseMatrix_ELL : public SparseMatrix<fp_type>
    {
        public:
            SparseMatrix_ELL(const size_t nrows, const size_t ncols);
            void assemble();
            void disassemble();
            
            std::vector<fp_type> matvec(const std::vector<fp_type>& x) const;

            size_t get_nrows() const noexcept;
            size_t get_ncols() const noexcept;
            size_t get_max_length_row() const noexcept;
            const std::vector<size_t>& get_col_idx() const noexcept;
            const std::vector<fp_type>& get_values() const noexcept;

            void set_col_idx(const std::vector<size_t>& col_idx);
            void set_values(const std::vector<fp_type>& values);
            void set_max_length_row(const size_t max_length_row);

            void view(std::ostream& os) const; 

            MatrixType       getFormat() const noexcept override;
            std::string_view printFormat() const noexcept override;

            ~SparseMatrix_ELL()= default;

        private:
            size_t max_length_row = 0;
            std::vector<size_t> col_idx;
            std::vector<fp_type> values;

    };
}

#endif