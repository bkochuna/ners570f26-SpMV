
#include "SpMV.hpp"

#include <sstream> // std::ostringstream, std::istringstream
#include <string>  // std::string, std::stod
#include <vector>  // std::vector

#include "unit_test_framework.hpp"

TEST_CASE(test_DEN_class)
{
    SpMV::SparseMatrix_DEN<double> A(2, 3);

    // Verify the number of rows and columns.
    ASSERT(A.getNrows() == 2);
    ASSERT(A.getNcols() == 3);

    // Verify the initial matrix state.
    ASSERT(A.getState() == SpMV::MatrixState::initialized);
}

TEST_SUITE(DEN_suite)
{
    TEST(test_DEN_class);
}

//==============================================================================
// view() test cases (Issue #52)
//==============================================================================

// Pull every number out of the text printed by view(). Brackets, commas, and
// other punctuation around a number are ignored, so the tests do not depend
// on the exact layout chosen in #48.
static std::vector<double> numbersIn(const std::string& text)
{
    std::vector<double> nums;
    std::istringstream words(text);
    std::string w;
    while (words >> w) {
        const auto first = w.find_first_of("0123456789-.");
        const auto last  = w.find_last_of("0123456789.");
        if (first == std::string::npos || last == std::string::npos || last < first)
            continue;
        try {
            size_t used = 0;
            const std::string num = w.substr(first, last - first + 1);
            const double val = std::stod(num, &used);
            if (used == num.size())
                nums.push_back(val);
        } catch (...) {
            // not a number, skip it
        }
    }
    return nums;
}

// True if every entry of the matrix, in row-major order (zeros included),
// appears back-to-back somewhere in the printed numbers.
template <typename T>
static bool printsAllEntries(const SpMV::SparseMatrix_DEN<T>& A, const std::string& text)
{
    std::vector<double> expected;
    for (size_t i = 0; i < A.getNrows(); i++)
        for (size_t j = 0; j < A.getNcols(); j++)
            expected.push_back(static_cast<double>(A.getValue(i, j)));

    const std::vector<double> nums = numbersIn(text);
    if (nums.size() < expected.size())
        return false;

    for (size_t start = 0; start + expected.size() <= nums.size(); start++) {
        bool match = true;
        for (size_t k = 0; k < expected.size() && match; k++) {
            const double diff = nums[start + k] - expected[k];
            match = (diff < 1e-6 && diff > -1e-6);
        }
        if (match)
            return true;
    }
    return false;
}

// Activity 1a: a 3x3 matrix with some zero entries prints every value,
// including the zeros, in the right order.
template <typename T>
TEST_CASE(view_square_with_zeros)
{
    SpMV::SparseMatrix_DEN<T> A(3, 3);
    A.setValue(0, 0, static_cast<T>(1.25));
    A.setValue(1, 1, static_cast<T>(2.5));
    A.setValue(2, 0, static_cast<T>(3.75));
    A.setValue(2, 2, static_cast<T>(4.125));
    A.assemble();

    std::ostringstream out;
    A.view(out);
    const std::string text = out.str();

    ASSERT(!text.empty());
    ASSERT(printsAllEntries(A, text));
}

// Activity 1a: a non-square (2x3) matrix prints rows and columns in the
// correct order, not transposed.
template <typename T>
TEST_CASE(view_non_square)
{
    SpMV::SparseMatrix_DEN<T> A(2, 3);
    A.setValue(0, 0, static_cast<T>(1.5));
    A.setValue(0, 1, static_cast<T>(2.5));
    A.setValue(0, 2, static_cast<T>(3.5));
    A.setValue(1, 0, static_cast<T>(4.5));
    A.setValue(1, 1, static_cast<T>(5.5));
    A.setValue(1, 2, static_cast<T>(6.5));
    A.assemble();

    std::ostringstream out;
    A.view(out);
    const std::string text = out.str();

    ASSERT(printsAllEntries(A, text));

    // The transposed order (1.5, 4.5, 2.5, ...) should not be what was printed
    const std::vector<double> nums = numbersIn(text);
    bool transposed = false;
    for (size_t k = 0; k + 1 < nums.size(); k++) {
        const double d0 = nums[k] - 1.5;
        const double d1 = nums[k + 1] - 4.5;
        if (d0 < 1e-6 && d0 > -1e-6 && d1 < 1e-6 && d1 > -1e-6)
            transposed = true;
    }
    ASSERT(!transposed);
}

// Activity 1b: changing a value after assembling and re-assembling shows the
// updated value in view().
template <typename T>
TEST_CASE(view_after_rebuild)
{
    SpMV::SparseMatrix_DEN<T> A(2, 2);
    A.setValue(0, 0, static_cast<T>(1.5));
    A.setValue(1, 1, static_cast<T>(2.5));
    A.assemble();

    A.disassemble();
    A.setValue(0, 1, static_cast<T>(9.75));
    A.assemble();

    std::ostringstream out;
    A.view(out);
    const std::string text = out.str();

    ASSERT_NEAR(A.getValue(0, 1), static_cast<T>(9.75), static_cast<T>(1e-6));
    ASSERT(printsAllEntries(A, text));

    // Viewing must not change the matrix
    ASSERT(A.getState() == SpMV::MatrixState::assembled);
}

template <typename T>
TEST_SUITE(DEN_view_suite)
{
    TEST(view_square_with_zeros<T>);
    TEST(view_non_square<T>);
    TEST(view_after_rebuild<T>);
}

auto main() -> int
{
    RUN_SUITE(DEN_suite);
    RUN_SUITE(DEN_view_suite<float>);
    RUN_SUITE(DEN_view_suite<double>);
    return 0;
}
