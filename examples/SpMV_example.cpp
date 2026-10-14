#include <iostream>
#include "SpMV.hpp"

int main(/* int argc, char* argv[] */)
{
    std::cout << "Hello World!" << std::endl;

    SpMV::SparseMatrix* ptr_A = new SpMV::SparseMatrix(20,20);

    // New scoping unit. This means variables defined in here, stay here.
    {
        SpMV::SparseMatrix A = SpMV::SparseMatrix(10000,-10);
        std::cout << "Lets do stuff to A!" << std::endl;
    }

    delete(ptr_A);
    ptr_A = NULL;

    return 0;
}
