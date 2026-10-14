#include <iostream>
#include "SpMV.hpp"

int main(int arg, char* argv[])
{
    std::cout << "Hello World!" << std::endl;

    SpMV::SparseMatrix* ptr_A = new SpMV::SparseMatrix();

    // New scoping unit. This means variables defined in here, stay here.
    {
        SpMV::SparseMatrix A = SpMV::SparseMatrix();
        std::cout << "Lets do stuff to A!" << std::endl;
    }

    delete(ptr_A);

    return 0;
}
