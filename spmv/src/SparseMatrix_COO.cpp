#include "SparseMatrix_COO.hpp"

#include <iostream>
#include <stddef.h>

#include <cassert>
#include <algorithm>

using namespace std;

namespace SpMV
{
    /**
     * @brief Constructs SparseMatrix_COO with default constructor
     *
     * Only calls base-class (SparseMatrix) constructor
     *
     * @param nrows Number of rows in the matrix
     * @param ncols Number of columns in the matrix
     *
     * @pre deferred to base class
     * @post deferred to base class
     */
    template <class fp_type>
    SparseMatrix_COO<fp_type>::SparseMatrix_COO(const std::size_t nrows, const std::size_t ncols) : 
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
    SparseMatrix_COO<fp_type>::SparseMatrix_COO(const std::size_t nnz, const std::size_t* ia, const std::size_t* ja, const fp_type* vals) : 
        SparseMatrix<fp_type>(1,1)
    {
        // DBC REQUIRE assertions on input args
        assert(nnz > 0);
        assert(ia != nullptr);
        assert(ja != nullptr);
        assert(vals != nullptr);

        this->_numnz = nnz;
        // Allocate
        this->_ia = new std::size_t[nnz];
        this->_ja = new std::size_t[nnz];
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
        assert(this->getState() == MatrixState::assembled);
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
    SparseMatrix_COO<fp_type>::SparseMatrix_COO(const std::vector<size_t>& ia, const std::vector<size_t>& ja, const std::vector<fp_type>& vals) :
        SparseMatrix<fp_type>(1,1)
    {
        // DBC REQUIRE assertions on input args
        assert(ia.size() > 0);
        assert(ia.size() == ja.size());
        assert(ia.size() == vals.size());

        this->_numnz = ia.size();
        
        // Allocate
        this->_ia = new std::size_t[this->_numnz];
        this->_ja = new std::size_t[this->_numnz];
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
        assert(this->getState() == MatrixState::assembled);
    }

    /**
     * @brief Copy constructor
     *
     * Creates a `SparseMatrix_COO` by deep-copying another `SparseMatrix_COO`
     *
     * @param that the matrix to copy
     *
     * @pre the other matrix exists
     * @post two matrices are equal (TBD -- need to overload == operator)
     */
    template <class fp_type>
    SparseMatrix_COO<fp_type>::SparseMatrix_COO(const SparseMatrix_COO& that) :
        SparseMatrix<fp_type>(that)
    {
        if(that.getState() >= MatrixState::assembled) {
            this->_numnz = that._numnz;
            // Allocate
            this->_ia = new size_t[this->_numnz];
            this->_ja = new size_t[this->_numnz];
            this->_vals = new fp_type[this->_numnz];
             // Copy
            copy_n(that._ia,this->_numnz,this->_ia);
            copy_n(that._ja,this->_numnz,this->_ja);
            copy_n(that._vals,this->_numnz,this->_vals);
        }
        
    }

    /**
     * @brief Overloads assignment operator for copying
     *
     * Performs a deep-copy during assignment with `A = B`
     * 
     * @param that the LHS matrix to copy
     *
     * @return a copy of the RHS Sparse Matrix
     *
     * @pre TBD
     * @post TBD
     */
    template <class fp_type>
    SparseMatrix_COO<fp_type>& SparseMatrix_COO<fp_type>::operator=(const SparseMatrix_COO<fp_type>& that)
    {
        if(this == &that) {
            return *this;
        }
        
        // Create a local copy
        SparseMatrix_COO a_copy(that);
        
        // Swap the base-class state.
        swap(this->_nrows, a_copy._nrows);
        swap(this->_ncols, a_copy._ncols);

        swap(this->_numnz, a_copy._numnz);
        swap(this->_ia, a_copy._ia);
        swap(this->_ja, a_copy._ja);
        swap(this->_vals, a_copy._vals);
        return *this;
    }
    
    /**
     * @brief Move constructor
     *
     * Transfers ownership of data in `that` to `this`.
     * e.g., `SparseMatrix_COO b(std::move(a));`
     * 
     * @param that the LHS matrix to copy
     *
     * @return a copy of the RHS Sparse Matrix
     *
     * @pre TBD
     * @post TBD
     */
    template <class fp_type>
    SparseMatrix_COO<fp_type>::SparseMatrix_COO(SparseMatrix_COO&& that) noexcept :
        SparseMatrix<fp_type>(std::move(that)),
        _ia(that._ia), _ja(that._ja), _vals(that._vals)
    {
        that._ia = nullptr;
        that._ja = nullptr;
        that._vals = nullptr;
    }

    /**
     * @brief COO Destructor
     *
     * Must delete `_ia`, `_ja`, and `_vals` attributes. 
     */
    template <class fp_type>
    SparseMatrix_COO<fp_type>::~SparseMatrix_COO()
    {
        delete[] this->_ia; this->_ia = nullptr;
        delete[] this->_ja; this->_ja = nullptr;
        delete[] this->_vals; this->_vals = nullptr;
        // Base class destructor executes next
    }
    
    /**
     * @brief Converts COO formatted data to base class storage
     *
     * Specifically converts `_ia`, `_ja`, and `vals` into the base-class storage
     *  format defined by `_buildCoeff
     *
     * @pre matrix has been built
     * @post base class storage is restored, state is building
     */
    template <class fp_type>
    void SparseMatrix_COO<fp_type>::_disassemble() noexcept
    {

    }

    /**
     * @brief Converts matrix data to COO storage format
     *
     * Specifically converts the base-class storage format defined by
     * `_buildCoeff
     *
     * @pre values in the matrix have been set
     * @post base class storage is empty, state is assembled
     */
    template <class fp_type>
    void SparseMatrix_COO<fp_type>::assemble()
    {
        // DBC pre-conditions
        assert(this->getState() > MatrixState::initialized);
        assert(this->getNumnz > 0);

        // Assemble the matrix if its not assembled
        if(this->getState() == MatrixState::building)
        {
            // Allocate the private COO storage arrays
            this->_ia = new size_t[this->_numnz];
            this->_ja = new size_t[this->_numnz];
            this->_vals = new fp_type[this->_numnz];

            // Loop over entries in the map and assign to COO arrays
            size_t n=0;
            for (const auto& [ij, aij] : this->_buildCoeff) {
                const auto& [i,j] = ij;
                this->_ia[n] = i;
                this->_ja[n] = j;
                this->_vals[n] = aij;
                ++n;
            }

            // Clear the map
            this->_buildCoeff.clear();
        }

        // DBC post-conditions
        assert(this->getState() == MatrixState::assembled);
        assert(this->_buildCoeff.empty());
        assert(this->_ia != nullptr);
        assert(this->_ja != nullptr);
        assert(this->_vals != nullptr);
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
