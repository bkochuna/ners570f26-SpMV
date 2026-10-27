#ifndef __SPMV_SparseMatrix__
#define __SPMV_SparseMatrix__

#include <cstddef>      //for std::size_t
#include <map>          //for std::map
#include <utility>      //for std::pair
#include <string_view>  //for std::string_view
#include <vector>       //for std::vector

namespace SpMV
{

    enum class MatrixState {undefined, initialized, building, assembled};
    enum class MatrixType  {DEN, COO, CSR, ELL, JDS};

    template <class fp_type>
    class SparseMatrix
    {
    using key_type = std::pair<std::size_t, std::size_t>;
    using CoeffMap = std::map<key_type, fp_type>;  // Essentially defines an alias. Similar to a C typedef

    protected:
        MatrixState _state = MatrixState::undefined;
        std::size_t _nrows = 0;
        std::size_t _ncols = 0;
        std::size_t _numnz = 0;
           CoeffMap _buildCoeff;

        virtual void _disassemble() noexcept =0;
        
    public:
        // Default Constructor and Destructor
        SparseMatrix(const std::size_t nrows, const std::size_t ncols); 
        virtual ~SparseMatrix();

        // Copy and Move constructors
        // "= default" means automagically generate assignments for each attribute
        SparseMatrix(const SparseMatrix& that) = default;                 //Copy
        SparseMatrix& operator=(const SparseMatrix& that) = default;      //Copy assignment
        SparseMatrix(SparseMatrix&& that) noexcept;                       //Move
        SparseMatrix& operator=(SparseMatrix&& that) noexcept;            //Move assginment

        // Accessors
        [[nodiscard]] MatrixState     getState()    const noexcept { return _state; } //simple accessors are typically
        [[nodiscard]] std::size_t     getNrows()    const noexcept { return _nrows; } //written fully in header
        [[nodiscard]] std::size_t     getNcols()    const noexcept { return _ncols; }
        [[nodiscard]] std::size_t     getNumnz()    const noexcept { return _numnz; }
        [[nodiscard]] const CoeffMap& getCoeffMap() const noexcept { return _buildCoeff; }
        // ^          ^             ^ means this method returns a reference to _buildCoeff
        // |          |  means the reference that is returned is read only
        // | will generate a compiler warning if the method is used and the return arg is not used.
 
                              void    setValue(const std::size_t i, const std::size_t j, fp_type val);
        [[nodiscard]] virtual fp_type getValue(const std::size_t i, const std::size_t j) const noexcept;

        // Abstract methods
        virtual void              assemble()                   =0; // The "=0" defines this class as abstract
        virtual MatrixType        getFormat()   const noexcept =0; // const means "this" cannot be modified except for mutable members
        virtual std::string_view  printFormat() const noexcept =0; // noexcept means it does not throw any exceptions

        virtual fp_type*             matvec(const fp_type* x)             const noexcept =0;
        virtual std::vector<fp_type> matvec(const std::vector<fp_type> x) const noexcept =0;
    };
}

#endif
