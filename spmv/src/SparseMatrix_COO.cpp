#include "SparseMatrix_COO.hpp"

#include <iostream>
#include <stddef.h>

#include <cassert>
#include <algorithm>

using namespace std;

namespace SpMV
{

    template <class fp_type>
    SparseMatrix_COO<fp_type>::SparseMatrix_COO(const size_t nrows, const size_t ncols) : 
        SparseMatrix<fp_type>(nrows,ncols) {/* base class constructor only*/}

    /**
     * @brief Constructs SparseMatrix_COO from raw pointers
     *
     * Input arguments are basic COO storage arrays and dimension info. Row and
     * col dimensions are inferred from index arrays.
     *
     * @param nnz the number of nonzeros in matrix (size of ia, ja, and vals)
     * @param ia pointer to `nnz` row indices
     * @param ja pointer to `nnz` column indices
     * @param vals pointer to `nnz` nonzero values
     *
     * @pre `ia`, `ja`, and `vals` are not null, `nnz > 0`
     * @post constructed object owns copies of dummy arg arrays. State is assembled.
     */
    template <class fp_type>
    SparseMatrix_COO<fp_type>::SparseMatrix_COO(const size_t nnz, const size_t* ia, const size_t* ja, const fp_type* vals) : 
        SparseMatrix<fp_type>(1,1)
    {
        // DBC REQUIRE assertions on input args
        assert(nnz > 0);
        assert(ia != nullptr);
        assert(ja != nullptr);
        assert(vals != nullptr);

        this->_numnz = nnz;
        // Allocate
        this->_ia = new size_t[nnz];
        this->_ja = new size_t[nnz];
        this->_vals = new fp_type[nnz];
        // Copy
        copy_n(ia,this->_numnz,this->_ia);
        copy_n(ja,this->_numnz,this->_ja);
        copy_n(vals,this->_numnz,this->_vals);
        
        // Find number of rows and cols
        this->_nrows = *max_element(ia, ia + nnz)+1;
        this->_ncols = *max_element(ja, ja + nnz)+1;
        
        this->_state = MatrixState::assembled;

        // DBC ENSURE assertions on outputs
        assert(this->_numnz == nnz);
        assert(equal(this->_ia,this->_ia + this->_numnz, ia, ia + nnz));
        assert(equal(this->_ja,this->_ja + this->_numnz, ja, ja + nnz));
        assert(equal(this->_vals,this->_vals + this->_numnz, vals, vals + nnz));
        assert(this->getState() == assembled);
    }

    /**
     * @brief  Constructs SparseMatrix_COO from vectors
     *
     * Input arguments are vectors corresponding to basic COO storage arrays. Dimension
     * info is taken from vectors.
     *
     * @param ia the pointer array row indices as a vector
     * @param ja the pointer array for column indices as a vector
     * @param vals the pointer array for the nonzero matrix values as a vector
     *
     * @pre all vectors are equal length greater than 0
     * @post constructed object owns copies of dummy arg arrays. State is assembled.
     */
    template <class fp_type>
    SparseMatrix_COO<fp_type>::SparseMatrix_COO(const vector<size_t>& ia, const vector<size_t>& ja, const vector<fp_type>& vals) :
        SparseMatrix<fp_type>(1,1)
    {
        // DBC REQUIRE assertions on input args
        assert(ia.size() > 0);
        assert(ia.size() == ja.size());
        assert(ia.size() == vals.size());

        this->_numnz = ia.size();
        
        // Allocate
        this->_ia = new size_t[this->_numnz];
        this->_ja = new size_t[this->_numnz];
        this->_vals = new fp_type[this->_numnz];

        // Copy
        copy(ia.begin(), ia.end(), this->_ia);
        copy(ja.begin(), ja.end(), this->_ja);
        copy(vals.begin(), vals.end(), this->_vals);

        this->_state = MatrixState::assembled;

        // DBC ENSURE assertions on outputs
        assert(this->_numnz == ia.size());
        assert(equal(ia.begin(),ia.end(), this->_ia));
        assert(equal(ja.begin(),ja.end(), this->_ja));
        assert(equal(vals.begin(),vals.end(), this->_vals));
        assert(this->getState() == assembled);
    }

    template <class fp_type>
    void SparseMatrix_COO<fp_type>::assemble()
    {

    }

    /**
     * @brief Static function returns the child class matrix type
     * @return `MatrixType::COO`
     */
    template <class fp_type>
    MatrixType SparseMatrix_COO<fp_type>::getFormat() const noexcept
    {
        return MatrixType::COO;
    }

    /**
     * @brief Static function returns child class matrix type as a string
     * @return "COO" as a `std::string_view`
     */
    template <class fp_type>
    std::string_view SparseMatrix_COO<fp_type>::printFormat() const noexcept
    {
        return "COO";
    }
}

// Need to declare the concrete templates within the library for
// use in code that links to libspmv
template class SpMV::SparseMatrix_COO<float>;
template class SpMV::SparseMatrix_COO<double>;
