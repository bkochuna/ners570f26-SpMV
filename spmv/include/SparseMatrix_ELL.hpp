#ifndef __SPMV_SparseMatrix_ELL__
#define __SPMV_SparseMatrix_ELL__

#include "SparseMatrix.hpp"
#include <vector>
#include <cstddef>
#include <string_view>
#include <iosfwd>

namespace SpMV
{
    template <class fp_type>
    class SparseMatrix_ELL : public SparseMatrix<fp_type>
    {
        // Private variables for ELL
        private:
            // Variables for ELL format storage
            size_t max_length_row = 0;
            std::vector<size_t> col_idx;
            std::vector<fp_type> values;
            
        public:
            SparseMatrix_ELL(const size_t nrows, const size_t ncols);
            void assemble();
            void disassemble();

            std::vector<fp_type> matvec(const std::vector<fp_type>& x) const;
            
            // ==================================================================================
            // ==== Getters & Setters: Implemented by zoujanice
            // ==================================================================================
            // Getters for any matrix inspection
            size_t get_nrows() const noexcept;
            size_t get_ncols() const noexcept;
            size_t get_max_length_row() const noexcept;
            const std::vector<size_t>& get_col_idx_array() const noexcept;
            const std::vector<fp_type>& get_values_array() const noexcept;
            size_t get_col_idx(const size_t i, const size_t slot, const size_t totalRows = 0) const;
            fp_type get_value(const size_t i, const size_t slot, const size_t totalRows = 0) const;

            // Setters to change max row length
            void set_max_length_row(const size_t length) {
                max_length_row = length;
            }
            // ==================================================================================

            void view() const;
            ~SparseMatrix_ELL()= default;

            // From original code template
            MatrixType       getFormat() const noexcept override;
            std::string_view printFormat() const noexcept override;

    };
}

#endif
