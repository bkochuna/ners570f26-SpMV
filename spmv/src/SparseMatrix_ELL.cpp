#include "SparseMatrix_ELL.hpp"
#include <iostream> 
#include <vector>
#include <cstddef>
#include <string_view>
#include <iosfwd>
#include "SparseMatrix.hpp"

using namespace std;

namespace SpMV
{
    template <class fp_type>
    SparseMatrix_ELL<fp_type>::SparseMatrix_ELL(const size_t nrows, const size_t ncols):
        SparseMatrix<fp_type>::SparseMatrix(nrows, ncols)
    {
        cout << "Hello from ELL Constructor" << endl;
    }

    template <class fp_type>
    void SparseMatrix_ELL<fp_type>::assemble()
    {

    }

    template <class fp_type>
    void SparseMatrix_ELL<fp_type>::disassemble()
    {
        
    }

    template <class fp_type>
    std::vector<fp_type> matvec(const std::vector<fp_type>& x)
    {

    } 
    
    template <class fp_type>
    size_t get_nrows()
    {

    } 

    template <class fp_type>
    size_t get_ncols()
    {

    }

    template <class fp_type>
    size_t get_max_length_row()
    {

    }

    template <class fp_type>
    const std::vector<size_t>& get_col_idx()
    {

    }

    template <class fp_type>
    const std::vector<fp_type>& get_values()
    {

    }
    
    template <class fp_type>
    void set_col_idx(const std::vector<size_t>& col_idx)
    {

    }

    template <class fp_type>
    void set_values(const std::vector<fp_type>& values)
    {

    }

    template <class fp_type>
    void set_max_length_row(const size_t max_length_row)
    {

    }
    
    template <class fp_type>
    void view(std::ostream& os)
    {

    }

    template <class fp_type>
    MatrixType SparseMatrix_ELL<fp_type>::getFormat() const noexcept
    {
        return MatrixType::ELL;
    }

    template <class fp_type>
    std::string_view SparseMatrix_ELL<fp_type>::printFormat() const noexcept
    {
        return "ELL";
    }
}

template class SpMV::SparseMatrix_ELL<float>;
template class SpMV::SparseMatrix_ELL<double>;