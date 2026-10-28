#include "SparseMatrix_COO.hpp"

#include <iostream>
#include <stddef.h>

#include <cassert>
#include <algorithm>

using namespace std;

namespace SpMV
{

    using namespace std;

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
     * e.g., `SparseMatrix_COO b(move(a));`
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
        SparseMatrix<fp_type>(move(that)),_ia(that._ia), _ja(that._ja), _vals(that._vals)
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
         // DBC preconditions
        assert(this->getState() > MatrixState::assembled);
        assert(this->_ia != nullptr);
        assert(this->_ja != nullptr);
        assert(this->_vals != nullptr);

        // Loop over entries in COO arrays and assign to map
        for (size_t n = 0; n < this->getNumnz(); ++n) {
            auto i   = this->_ia[n];
            auto j   = this->_ja[n];
            fp_type aij = this->_vals[n];
            this->_buildCoeff[make_pair(i,j)] = aij;
        }

        // Deallocate the private COO storage arrays and set pointers to null.
        delete[] this->_ia;
        delete[] this->_ja;
        delete[] this->_vals;
        this->_ia   = nullptr;
        this->_ja   = nullptr;
        this->_vals = nullptr;

        this->_state = MatrixState::building;

        // DBC Post-conditions
        assert(this->getState() == MatrixState::building);
        assert(!this->_buildCoeff.empty());
        assert(this->_ia == nullptr);
        assert(this->_ja == nullptr);
        assert(this->_vals == nullptr);
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
        assert(this->getNumnz() > 0);

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
     * @brief Returns a specified value in the matrix
     *
     * Defaults to base class, but will override if state is assembled
     * 
     * @param i the row index
     * @param j the column index
     * 
     * @return fp_type the value in the matrix
     *
     * @pre deferred to base class
     * @post none
     */
    template <class fp_type>
    fp_type SparseMatrix_COO<fp_type>::getValue(const size_t i, const size_t j) const noexcept
    {
        // Initialize to base class condition for DBC and building,
        // returns 0.0 if matrix is assembled.
        fp_type aij = SparseMatrix<fp_type>::getValue(i, j);

        if (this->getState() == MatrixState::assembled)
        {
            // Search COO format
            for (size_t n = 0; n < this->getNumnz(); ++n)
            {
                if (this->_ia[n] == i && this->_ja[n] == j)
                {
                    aij = this->_vals[n];
                    break;
                }
            }
        }
        return aij;
    }

    /**
     * @brief Returns COO arrays as a tuple of pointers
     * 
     * @return tuple of const pointers
     *
     * @pre Matrix is assembled
     * @post pointers are not null
     */
    template <class fp_type>
    tuple<const size_t*, const size_t*, const fp_type*> SparseMatrix_COO<fp_type>::getCOOArrays() const noexcept
    {
        // Pre conditions
        assert(this->getState() == MatrixState::assembled);

        // Post conditions
        assert(this->_ia == nullptr);
        assert(this->_ja == nullptr);
        assert(this->_vals == nullptr);

        return {this->_ia, this->_ja, this->_vals};
    }

    /**
     * @brief Returns copies of COO arrays as a tuple of vectors
     * 
     * @return tuple of vectors
     *
     * @pre Matrix is assembled
     * @post pointers are not null
     */
    template <class fp_type>
    tuple<vector<size_t>, vector<size_t>, vector<fp_type>> SparseMatrix_COO<fp_type>::copyCOOArrays() const noexcept
    {
        // Pre conditions
        assert(this->getState() == MatrixState::assembled);

        // Post conditions
        assert(this->_ia == nullptr);
        assert(this->_ja == nullptr);
        assert(this->_vals == nullptr);

        return {
            vector<size_t>(this->_ia, this->_ia + this->getNumnz()),
            vector<size_t>(this->_ja, this->_ja + this->getNumnz()),
            vector<fp_type>(this->_vals, this->_vals + this->getNumnz())
        };
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
     * @return "COO" as a `string_view`
     */
    template <class fp_type>
    string_view SparseMatrix_COO<fp_type>::printFormat() const noexcept
    {
        return "COO";
    }

    /**
     * @brief Matvec operation for raw pointers
     *
     * Implements $y \gets Ax$ for $A$ stored in COO format.
     *
     * @param x the vector to multiply with A
     * @return the vector result of $Ax$
     *
     * @pre matrix is assembled x is not null.
     * @post None
     */
    template <class fp_type>
    fp_type* SparseMatrix_COO<fp_type>::matvec(const fp_type* x) const noexcept
    {
        assert(this->getState() == MatrixState::assembled);
        assert(x != nullptr);

        fp_type* y = new fp_type[this->_nrows]{};
        for(size_t n = 0; n < this->getNumnz(); ++n)
        {
            auto i   = this->_ia[n];
            auto j   = this->_ja[n];
            auto aij = this->_vals[n];

            y[i] += aij*x[j];
        }
        
        return y;
    }

    /**
     * @brief Matvec operation for raw pointers
     *
     * Implements $y \gets Ax$ for $A$ stored in COO format.
     *
     * @param x the vector to multiply with A
     * @return the vector result of $Ax$
     *
     * @pre matrix is assembled x same size as columns of A.
     * @post None
     */
    template <class fp_type>
    vector<fp_type> SparseMatrix_COO<fp_type>::matvec(const vector<fp_type> x) const noexcept
    {
        assert(this->getState() == MatrixState::assembled);
        assert(x.size() == this->getNcols());

        vector<fp_type> y(this->getNrows(),0.0);
        for(size_t n = 0 ; n < this->getNumnz(); ++n)
        {
            auto i   = this->_ia[n];
            auto j   = this->_ja[n];
            auto aij = this->_vals[n];

            y[i] += aij*x[j];
        }
        return y;
    }
}

// Need to declare the concrete templates within the library for
// use in code that links to libspmv
template class SpMV::SparseMatrix_COO<float>;
template class SpMV::SparseMatrix_COO<double>;
