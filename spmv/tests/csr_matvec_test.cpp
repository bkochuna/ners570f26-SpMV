#include "SpMV.hpp"
#include "SparseMatrix_CSR.hpp"

#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

#define TOL_DOUBLE 1e-12
#define TOL_FLOAT 1e-5f

template <class fp_type>
bool test_csr_matvec_basic(fp_type tol)
{
    SpMV::SparseMatrix_CSR<fp_type> A(3, 3);

    A.setValue(0, 0, static_cast<fp_type>(0.1));
    A.setValue(0, 2, static_cast<fp_type>(0.2));
    A.setValue(1, 1, static_cast<fp_type>(-0.3));
    A.setValue(2, 0, static_cast<fp_type>(0.4));
    A.setValue(2, 2, 0.5);
    A.assemble();

    vector<fp_type> vec = {static_cast<fp_type>(0.3),
                           static_cast<fp_type>(-0.2),
                           static_cast<fp_type>(0.7)};
    vector<fp_type> expected = {static_cast<fp_type>(0.17),
                                static_cast<fp_type>(0.06),
                                static_cast<fp_type>(0.47)};

    A.matvec(1, vec);

    if (vec.size() != expected.size()) {
        return false;
    }

    for (size_t i = 0; i < expected.size(); i++) {
        if (!(abs(vec[i] - expected[i]) <= tol)) {
            return false;
        }
    }

    // Test zero input vector.
    vec = {0.0, 0.0, 0.0};
    A.matvec(1, vec);

    if (vec.size() != 3) {
        return false;
    }

    for (size_t i = 0; i < vec.size(); i++) {
        if (!(abs(vec[i]) <= tol)) {
            return false;
        }
    }

    return true;
}

template <class fp_type>
bool test_csr_matvec_identity(fp_type tol)
{
    SpMV::SparseMatrix_CSR<fp_type> A(3, 3);

    for (size_t i = 0; i < 3; i++) {
        A.setValue(i, i, 1.0);
    }
    A.assemble();

    vector<fp_type> vec = {2.0, -3.0, 0.5};
    vector<fp_type> expected = vec;

    A.matvec(1, vec);

    if (vec.size() != expected.size()) {
        return false;
    }

    for (size_t i = 0; i < expected.size(); i++) {
        if (!(abs(vec[i] - expected[i]) <= tol)) {
            return false;
        }
    }

    return true;
}

template <class fp_type>
bool test_csr_matvec_empty_rows(fp_type tol)
{
    SpMV::SparseMatrix_CSR<fp_type> A(5, 5);

    // Rows 0, 2, and 4 are empty.
    A.setValue(1, 0, 2.0);
    A.setValue(1, 2, -1.0);
    A.setValue(3, 1, 3.0);
    A.assemble();

    vector<fp_type> vec = {1.0, 2.0, 4.0, 0.0, 0.0};
    vector<fp_type> expected = {0.0, -2.0, 0.0, 6.0, 0.0};

    A.matvec(1, vec);

    if (vec.size() != expected.size()) {
        return false;
    }

    for (size_t i = 0; i < expected.size(); i++) {
        if (!(abs(vec[i] - expected[i]) <= tol)) {
            return false;
        }
    }

    return true;
}

template <class fp_type>
bool test_csr_matvec_repeated(fp_type tol)
{
    SpMV::SparseMatrix_CSR<fp_type> A(2, 2);

    A.setValue(0, 0, 1.0);
    A.setValue(0, 1, 2.0);
    A.setValue(1, 0, 3.0);
    A.setValue(1, 1, 4.0);
    A.assemble();

    vector<fp_type> vec = {1.0, 2.0};
    vector<fp_type> expected = {27.0, 59.0};

    // Compute A^2 * vec.
    A.matvec(2, vec);

    if (vec.size() != expected.size()) {
        return false;
    }

    for (size_t i = 0; i < expected.size(); i++) {
        if (!(abs(vec[i] - expected[i]) <= tol)) {
            return false;
        }
    }

    return true;
}

template <class fp_type>
bool run_csr_matvec_tests(fp_type tol)
{
    bool all_passed = true;

    if (test_csr_matvec_basic<fp_type>(tol)) {
        cout << "Basic sparse matrix: PASSED" << endl;
    } else {
        cout << "Basic sparse matrix: FAILED" << endl;
        all_passed = false;
    }

    if (test_csr_matvec_identity<fp_type>(tol)) {
        cout << "Identity matrix: PASSED" << endl;
    } else {
        cout << "Identity matrix: FAILED" << endl;
        all_passed = false;
    }

    if (test_csr_matvec_empty_rows<fp_type>(tol)) {
        cout << "Empty rows: PASSED" << endl;
    } else {
        cout << "Empty rows: FAILED" << endl;
        all_passed = false;
    }

    if (test_csr_matvec_repeated<fp_type>(tol)) {
        cout << "Repeated multiplication: PASSED" << endl;
    } else {
        cout << "Repeated multiplication: FAILED" << endl;
        all_passed = false;
    }

    return all_passed;
}

int main()
{
    cout << "Testing CSR matvec<float>:" << endl;
    bool float_passed = run_csr_matvec_tests<float>(TOL_FLOAT);

    cout << endl;

    cout << "Testing CSR matvec<double>:" << endl;
    bool double_passed = run_csr_matvec_tests<double>(TOL_DOUBLE);

    if (float_passed && double_passed) {
        cout << "All CSR matvec tests passed." << endl;
        return 0;
    } else {
        cout << "Some CSR matvec tests failed." << endl;
        return 1;
    }
}
