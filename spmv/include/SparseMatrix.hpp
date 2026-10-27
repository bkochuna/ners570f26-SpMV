#ifndef __SPMV_SparseMatrix__
#define __SPMV_SparseMatrix__

#include <cstddef>      //for std::size_t
#include <map>          //for std::map
#include <utility>      //for std::pair
#include <string_view>  //for std::string_view

using namespace std;

namespace SpMV
{

    enum MatrixState {undefined, initialized, building, assembled};
    enum MatrixType  {DEN, COO, CSR, ELL, JDS};

    template <class fp_type>
    class SparseMatrix
    {
    using key_type = pair<size_t, size_t>;
    using CoeffMap = map<key_type, fp_type>;  // Essentially defines an alias. Similar to a C typedef


    protected:
        MatrixState _state = MatrixState::undefined;
             size_t _nrows = 0;
             size_t _ncols = 0;
             size_t _numnz = 0;
           CoeffMap _buildCoeff;
        
    public:
        // Default Constructor and Destructor
        SparseMatrix(const size_t nrows, const size_t ncols); 
        virtual ~SparseMatrix();

        // Copy and Move constructors
        // "= default" means automagically generate assignments for each attribute
        SparseMatrix(const SparseMatrix& that) = default;                 //Copy
        SparseMatrix& operator=(const SparseMatrix& that) = default;      //Copy assignment
        SparseMatrix(SparseMatrix&& that) noexcept = default;             //Move
        SparseMatrix& operator=(SparseMatrix&& that) noexcept = default;  //Move assginment

        // Accessors
        [[nodiscard]] MatrixState     getState()    const noexcept { return _state; } //simple accessors are typically
        [[nodiscard]] size_t          getNrows()    const noexcept { return _nrows; } //written fully in header
        [[nodiscard]] size_t          getNcols()    const noexcept { return _ncols; }
        [[nodiscard]] size_t          getNumnz()    const noexcept { return _numnz; }
        [[nodiscard]] const CoeffMap& getCoeffMap() const noexcept { return _buildCoeff; }
        // ^          ^             ^ means this method returns a reference to _buildCoeff
        // |          |  means the reference that is returned is read only
        // | will generate a compiler warning if the method is used and the return arg is not used.
 
                      void    setValue(const size_t i, const size_t j, fp_type val);
        [[nodiscard]] fp_type getValue(const size_t i, const size_t j) const noexcept;

        // Abstract methods
        virtual void         assemble()                   =0; // The "=0" defines this class as abstract
        virtual MatrixType   getFormat()   const noexcept =0; // const means "this" cannot be modified except for mutable members
        virtual string_view  printFormat() const noexcept =0; // noexcept means it does not throw any exceptions
    };
}

#endif
