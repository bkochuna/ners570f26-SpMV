#include "SpMV.hpp"
#include "SparseMatrix_CSR.hpp"

#include <cmath>
#include <cstddef>
#include <vector>

// Include the testing framework after all other headers.
#include "unit_test_framework.hpp"

namespace
{
    template <class fp_type>
    constexpr fp_type matvec_tolerance = static_cast<fp_type>(1e-12);

    template <>
    constexpr float matvec_tolerance<float> = 1e-5f;
}

template <class fp_type>
TEST_CASE(test_csr_matvec_basic)
{
    SpMV::SparseMatrix_CSR<fp_type> A(3, 3);

    A.setValue(0, 0, static_cast<fp_type>(0.1));
    A.setValue(0, 2, static_cast<fp_type>(0.2));
    A.setValue(1, 1, static_cast<fp_type>(-0.3));
    A.setValue(2, 0, static_cast<fp_type>(0.4));
    A.setValue(2, 2, static_cast<fp_type>(0.5));
    A.assemble();

    std::vector<fp_type> vec = {static_cast<fp_type>(0.3),
                                static_cast<fp_type>(-0.2),
                                static_cast<fp_type>(0.7)};
    const std::vector<fp_type> expected = {static_cast<fp_type>(0.17),
                                          static_cast<fp_type>(0.06),
                                          static_cast<fp_type>(0.47)};

    A.matvec(1, vec);

    ASSERT(vec.size() == expected.size());

    for (std::size_t i = 0; i < expected.size(); i++) {
        ASSERT_NEAR(vec[i], expected[i], matvec_tolerance<fp_type>);
    }

    // Test zero input vector.
    vec.assign(3, static_cast<fp_type>(0.0));
    A.matvec(1, vec);

    ASSERT(vec.size() == expected.size());

    for (std::size_t i = 0; i < vec.size(); i++) {
        ASSERT_NEAR(vec[i], static_cast<fp_type>(0.0), matvec_tolerance<fp_type>);
    }
}

template <class fp_type>
TEST_CASE(test_csr_matvec_identity)
{
    SpMV::SparseMatrix_CSR<fp_type> A(3, 3);

    for (std::size_t i = 0; i < 3; i++) {
        A.setValue(i, i, static_cast<fp_type>(1.0));
    }
    A.assemble();

    std::vector<fp_type> vec = {static_cast<fp_type>(2.0),
                                static_cast<fp_type>(-3.0),
                                static_cast<fp_type>(0.5)};
    const std::vector<fp_type> expected = vec;

    A.matvec(1, vec);

    ASSERT(vec.size() == expected.size());

    for (std::size_t i = 0; i < expected.size(); i++) {
        ASSERT_NEAR(vec[i], expected[i], matvec_tolerance<fp_type>);
    }
}

template <class fp_type>
TEST_CASE(test_csr_matvec_empty_rows)
{
    SpMV::SparseMatrix_CSR<fp_type> A(5, 5);

    // Rows 0, 2, and 4 are empty.
    A.setValue(1, 0, static_cast<fp_type>(2.0));
    A.setValue(1, 2, static_cast<fp_type>(-1.0));
    A.setValue(3, 1, static_cast<fp_type>(3.0));
    A.assemble();

    std::vector<fp_type> vec = {static_cast<fp_type>(1.0),
                                static_cast<fp_type>(2.0),
                                static_cast<fp_type>(4.0),
                                static_cast<fp_type>(0.0),
                                static_cast<fp_type>(0.0)};
    const std::vector<fp_type> expected = {static_cast<fp_type>(0.0),
                                          static_cast<fp_type>(-2.0),
                                          static_cast<fp_type>(0.0),
                                          static_cast<fp_type>(6.0),
                                          static_cast<fp_type>(0.0)};

    A.matvec(1, vec);

    ASSERT(vec.size() == expected.size());

    for (std::size_t i = 0; i < expected.size(); i++) {
        ASSERT_NEAR(vec[i], expected[i], matvec_tolerance<fp_type>);
    }
}

template <class fp_type>
TEST_CASE(test_csr_matvec_repeated)
{
    SpMV::SparseMatrix_CSR<fp_type> A(2, 2);

    A.setValue(0, 0, static_cast<fp_type>(1.0));
    A.setValue(0, 1, static_cast<fp_type>(2.0));
    A.setValue(1, 0, static_cast<fp_type>(3.0));
    A.setValue(1, 1, static_cast<fp_type>(4.0));
    A.assemble();

    std::vector<fp_type> vec = {static_cast<fp_type>(1.0),
                                static_cast<fp_type>(2.0)};
    const std::vector<fp_type> expected = {static_cast<fp_type>(27.0),
                                          static_cast<fp_type>(59.0)};

    // Compute A^2 * vec.
    A.matvec(2, vec);

    ASSERT(vec.size() == expected.size());

    for (std::size_t i = 0; i < expected.size(); i++) {
        ASSERT_NEAR(vec[i], expected[i], matvec_tolerance<fp_type>);
    }
}

template <class fp_type>
TEST_SUITE(csr_matvec_suite)
{
    TEST(test_csr_matvec_basic<fp_type>);
    TEST(test_csr_matvec_identity<fp_type>);
    TEST(test_csr_matvec_empty_rows<fp_type>);
    TEST(test_csr_matvec_repeated<fp_type>);
}

int main()
{
    RUN_SUITE(csr_matvec_suite<float>);
    RUN_SUITE(csr_matvec_suite<double>);
    return 0;
}
