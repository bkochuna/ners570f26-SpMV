#include "SpMV.hpp"
#include "SparseMatrix.hpp"
#include "SparseMatrix_JDS.hpp"

// Testing library required for testing (Always include this last!)
#include "unit_test_framework.hpp"

// Need these for viewing
#include <sstream>
#include <string>

// Test initialization
template <typename T> TEST_CASE(test_initialization) {
  // Standard initializations
  SpMV::SparseMatrix_JDS<T> test(3, 6);

  // Edge Cases
  SpMV::SparseMatrix_JDS<T> test_0(0, 0);
  SpMV::SparseMatrix_JDS<T> test_1(1, 1);
}

// Test Formats
template <typename T> TEST_CASE(test_get_format) {
  SpMV::SparseMatrix_JDS<T> test(2, 2);
  ASSERT(test.getFormat() == SpMV::MatrixType::JDS);
  ASSERT(test.printFormat() == "JDS");
}

// Test State
template <typename T> TEST_CASE(test_state) {
  SpMV::SparseMatrix_JDS<T> test(4, 4);

  ASSERT(test.getState() == SpMV::MatrixState::initialized);
  ASSERT(test.getCoeffMap().empty());
}

// Test Dimensions
template <typename T> TEST_CASE(test_dim) {
  SpMV::SparseMatrix_JDS<T> test(6, 9);
  auto nrows = test.getNrows();
  auto ncols = test.getNcols();
  auto nnz = test.getNumnz();

  ASSERT(nrows == 6);
  ASSERT(ncols == 9);
  ASSERT(nnz == 0);
}

// Test that the JDS matrix view displays readable information - Steven
template <typename T> TEST_CASE(test_view)
{
    // Create a 3x3 JDS matrix
    SpMV::SparseMatrix_JDS<T> test(3, 3);

    // Insert values into the matrix
    test.setValue(0, 0, T{1.0});
    test.setValue(0, 2, T{2.0});
    test.setValue(1, 1, T{3.0});

    // Assemble the matrix to convert it into JDS format
    test.assemble();

    // Capture the view output in a string stream
    std::ostringstream output;
    // Call the view method to display the matrix information
    test.view(output);

    // Store the output in a string
    std::string result = output.str();

    // Check that the output contains readable matrix information
    ASSERT(result.find("JDS matrix") != std::string::npos);
    ASSERT(result.find("Dimensions: 3 x 3") != std::string::npos);
    ASSERT(result.find("Nonzeros: 3") != std::string::npos);
    ASSERT(result.find("State: assembled") != std::string::npos);
    ASSERT(result.find("row_srt_idx") != std::string::npos);
    ASSERT(result.find("row_map") == std::string::npos);
    ASSERT(result.find("iter_ptr:") != std::string::npos);
    ASSERT(result.find("col_idx:") != std::string::npos);
    ASSERT(result.find("value:") != std::string::npos);

    // Check that the output strings appear in the correct order
    const size_t pos_matrix = result.find("JDS matrix");
    const size_t pos_dimensions = result.find("Dimensions: 3 x 3");
    const size_t pos_nonzeros = result.find("Nonzeros: 3");
    const size_t pos_state = result.find("State: assembled");
    const size_t pos_row_srt_idx = result.find("row_srt_idx");
    const size_t pos_iter_ptr = result.find("iter_ptr:");
    const size_t pos_col_idx = result.find("col_idx:");
    const size_t pos_value = result.find("value:");

    // Ensure that the positions of the strings are in the correct order
    ASSERT(pos_matrix < pos_dimensions);
    ASSERT(pos_dimensions < pos_nonzeros);
    ASSERT(pos_nonzeros < pos_state);
    ASSERT(pos_state < pos_row_srt_idx);
    ASSERT(pos_row_srt_idx < pos_iter_ptr);
    ASSERT(pos_iter_ptr < pos_col_idx);
    ASSERT(pos_col_idx < pos_value);

    // Count occurrences of a string in the output to avoid duplicates
    auto countOccurrences = [&](const std::string& text)
    {
        size_t count = 0;
        size_t pos = 0;

        // Count occurrences of the specified text in the result string
        while ((pos = result.find(text, pos)) != std::string::npos)
        {
            count++;
            pos += text.length();
        }

        return count;
    };

    // Check that each label appears exactly once
    ASSERT(countOccurrences("JDS matrix") == 1);
    ASSERT(countOccurrences("Dimensions:") == 1);
    ASSERT(countOccurrences("Nonzeros:") == 1);
    ASSERT(countOccurrences("State: assembled") == 1);
    ASSERT(countOccurrences("row_srt_idx") == 1);
    ASSERT(countOccurrences("row_map") == 0);
    ASSERT(countOccurrences("iter_ptr:") == 1);
    ASSERT(countOccurrences("col_idx:") == 1);
    ASSERT(countOccurrences("value:") == 1);

    // Check that the actual JDS data matches the input matrix
    ASSERT(result.find("row_srt_idx (sorted to original): 0 1 2") != std::string::npos);
    ASSERT(result.find("iter_ptr: (jagged diagonal offsets) 0 2 3") != std::string::npos);
    ASSERT(result.find("col_idx: (column indices of stored values) 0 1 2") != std::string::npos);
    ASSERT(result.find("value: (nonzero values in jagged diagonal order) 1 3 2") != std::string::npos);
}

// Basic Test Suite
template <typename T> TEST_SUITE(basic_suite) {
  TEST(test_initialization<T>);
  TEST(test_get_format<T>);
  TEST(test_state<T>);
  TEST(test_dim<T>);
  TEST(test_view<T>);
}

// Accessor Test cases
template <typename T> TEST_CASE(test_set) {
  // Testing whether setValue works on its own (does not throw any exceptions) and that the matrix can be assembled after
  SpMV::SparseMatrix_JDS<T> test(3, 3);
  test.setValue(0, 0, static_cast<T>(1));
  test.setValue(0, 2, static_cast<T>(2.35));
  test.setValue(1, 1, static_cast<T>(-3));
  test.setValue(2, 0, static_cast<T>(4.1));
  test.setValue(2, 1, static_cast<T>(5));
  test.setValue(2, 2, static_cast<T>(-6));
  test.assemble();
}

template <typename T> TEST_CASE(test_get) {
  SpMV::SparseMatrix_JDS<T> test(3, 3);
  // A = [1 0 2.35] example matrix used for all accessor tests
  //     [0 -3 0]
  //     [4.1 5 -6]
  // Setting values and assembling this matrix
  test.setValue(0, 0, static_cast<T>(1));
  test.setValue(0, 2, static_cast<T>(2.35));
  test.setValue(1, 1, static_cast<T>(-3));
  test.setValue(2, 0, static_cast<T>(4.1));
  test.setValue(2, 1, static_cast<T>(5));
  test.setValue(2, 2, static_cast<T>(-6));
  test.assemble();

  // Testing whether the values that are set above are retrieved accurately 
  ASSERT_NEAR(test.getValue(0,0),static_cast<T>(1),static_cast<T>(1e-3));
  ASSERT_NEAR(test.getValue(0,2),static_cast<T>(2.35),static_cast<T>(1e-3));
  ASSERT_NEAR(test.getValue(1,1),static_cast<T>(-3),static_cast<T>(1e-3));
  ASSERT_NEAR(test.getValue(2,0),static_cast<T>(4.1),static_cast<T>(1e-3));
  ASSERT_NEAR(test.getValue(2,1),static_cast<T>(5),static_cast<T>(1e-3));
  ASSERT_NEAR(test.getValue(2,2),static_cast<T>(-6),static_cast<T>(1e-3));

  // Testing whether values re-set after assembly are retrieved accurately
  test.setValue(0,0,static_cast<T>(5.7));
  test.assemble(); //setValue implicitly disassembles, so must reassemble
  ASSERT_NEAR(test.getValue(0,0),static_cast<T>(5.7),static_cast<T>(1e-3));
  test.setValue(0,2,static_cast<T>(-.1));
  test.assemble();
  ASSERT_NEAR(test.getValue(0,2),static_cast<T>(-.1),static_cast<T>(1e-3));
}

template <typename T> TEST_CASE(test_preassembly_re_set) {
  SpMV::SparseMatrix_JDS<T> test(3, 3);
  // Testing whether resetting a value before assembling works correctly 
  test.setValue(2, 2, static_cast<T>(2));
  test.setValue(2, 2, static_cast<T>(-6));
  test.assemble();
  ASSERT_NEAR(test.getValue(2,2),static_cast<T>(-6),static_cast<T>(1e-3));
}

template <typename T> TEST_CASE(test_preassembly_get) {
  SpMV::SparseMatrix_JDS<T> test(3, 3);
  // Testing whether getting a value before assembling works correctly (retrieves it from buildCoeff)
  test.setValue(2, 2, static_cast<T>(2));
  ASSERT_NEAR(test.getValue(2,2),static_cast<T>(2),static_cast<T>(1e-3));
}

template <typename T> TEST_CASE(test_get_value_empty) {
  // Testing whether initializing and assembling an empty matrix returns 0 
  SpMV::SparseMatrix_JDS<T> test(3, 3); 
  test.assemble();
  ASSERT_NEAR(test.getValue(0,0), static_cast<T>(0), static_cast<T>(1e-3)); 
}

template <typename T> TEST_SUITE(accessor_suite) {
  TEST(test_set<T>);
  TEST(test_get<T>);
  TEST(test_preassembly_get<T>);
  TEST(test_preassembly_re_set<T>);
  TEST(test_get_value_empty<T>);
}

// Assembly/disassembly unit tests - Hanga

// This tests a nominal use case where the user assembles a
// 5x5 sparse matrix, then explicitly disassembles it. This 
// test also explicitly tests if the matrix values are correct
// after assembly and disassembly.
template <typename T>
TEST_CASE(nominalAssembleDisassemble)
{
    // testing the nominal use case
    // using a predefined 5x5 lower triangular matrix
    // except for an empty row at row index 3
    size_t m = 5; // nrows
    size_t n = 5; // ncols

    // create JDS matrix instance
    SpMV::SparseMatrix_JDS<T> test_matrix(m, n);

    // matrix entry - starts at 1.0
    T mij = 1.0;
    // matrix entry taken from assembled matrix later
    T test_mij = 0.0;

    // set test matrix values
    for (size_t i=0; i<m; i++)
    {
        if (i!=3)
        {
            for (size_t j=0; j<=i; j++)
            {
                test_matrix.setValue(i, j, mij);
                mij += static_cast<T>(1.0);
            }

        }
    }

    // assemble matrix
    test_matrix.assemble();

    // see if it assembled
    ASSERT(test_matrix.getState() == SpMV::MatrixState::assembled);

    // test if values are what they should be
    mij = 1.0;
    for (size_t i=0; i<5; i++)
    {
        if (i!=3)
        {
            for (size_t j=0; j<n; j++)
            {
                // make sure nonzero values are where they should be
                // and are what they should be
                if (j<=i)
                {
                    test_mij = test_matrix.getValue(i, j);
                    ASSERT_NEAR(mij, test_mij, static_cast<T>(1e-3));
                    mij += static_cast<T>(1.0);
                }
                // also test that values that weren't explicitly set are zero
                else
                {
                    test_mij = test_matrix.getValue(i, j);
                    ASSERT_NEAR(static_cast<T>(0.0), test_mij, static_cast<T>(1e-3));
                }
            }

        }
        else // testing that the empty row is full of only zeros
        {
            for (size_t j=0; j<n; j++)
            {
                test_mij = test_matrix.getValue(i, j);
                ASSERT_NEAR(static_cast<T>(0.0), test_mij, static_cast<T>(1e-3));
            }
        }
    }

    // disassemble matrix
    test_matrix.disassemble();
    
    // see if it disassembled properly
    ASSERT(test_matrix.getState() == SpMV::MatrixState::building);

    // test if values are *still* what they should be
    mij = 1.0;
    for (size_t i=0; i<5; i++)
    {
        if (i!=3)
        {
            for (size_t j=0; j<n; j++)
            {
                // make sure nonzero values are where they should be
                // and are what they should be
                if (j<=i)
                {
                    test_mij = test_matrix.getValue(i, j);
                    ASSERT_NEAR(mij, test_mij, static_cast<T>(1e-3));
                    mij += static_cast<T>(1.0);
                }
                // also test that values that weren't explicitly set are zero
                else
                {
                    test_mij = test_matrix.getValue(i, j);
                    ASSERT_NEAR(static_cast<T>(0.0), test_mij, static_cast<T>(1e-3));
                }
            }

        }
        else // testing that the empty row is full of only zeros
        {
            for (size_t j=0; j<n; j++)
            {
                test_mij = test_matrix.getValue(i, j);
                ASSERT_NEAR(static_cast<T>(0.0), test_mij, static_cast<T>(1e-3));
            }
        }
    }
    // test matrix will be destroyed automatically once out of scope
}


// This tests a case where the user assembles a simple matrix
// (a 2x2 identity matrix in this case), then calls setValue,
// implictly disassembling it. In particular, we're making sure
// the matrix remains disassembled after setValue is called.
template <typename T>
TEST_CASE(assembleImpliedDisassemble)
{
    // testing if disassemble works when setValue called
    // just using a simple 2x2 identity matrix for this one

    size_t m = 2; // nrows & ncols

    // create JDS matrix instance
    SpMV::SparseMatrix_JDS<T> id_matrix(m, m);

    // set identity matrix values
    for (size_t i=0; i<m; i++)
    {
        id_matrix.setValue(i, i, static_cast<T>(1.0));
    }

    // assemble matrix
    id_matrix.assemble();
    // now call setValue on very first element
    id_matrix.setValue(0, 0, static_cast<T>(-1.0));
    // and check matrix state
    ASSERT(id_matrix.getState() == SpMV::MatrixState::building);
    // test matrix will be destroyed automatically once out of scope
}

// This tests a case where the user assembles a 2x2 zero matrix.
// In this case, we're just making sure that having an empty matrix
// doesn't cause any exceptions during assembly and disassembly.
// We already know the matrix state is updated correctly and that
// calling getValue on an empty matrix element returns zero.
template <typename T>
TEST_CASE(emptyAssembleDisassemble)
{
    // make 2x2 test matrix
    SpMV::SparseMatrix_JDS<T> test_matrix(2,2);
    // assemble it just to make sure it doesn't break
    // making sure its values are zero is handled by test_get_value_empty case
    test_matrix.assemble();
    // disassemble it to make sure it doesn't break
    test_matrix.disassemble();
    
}

// Suite to run all the assembly test cases for a templated type.
template <typename T>
TEST_SUITE(assemble_suite)
{
    TEST(nominalAssembleDisassemble<T>);
    TEST(assembleImpliedDisassemble<T>);
    TEST(emptyAssembleDisassemble<T>);
}


// Matrix-vector multiplication unit tests - Shuyi
// Check multiplication when row lengths differ.
template <typename T>
TEST_CASE(matvec_uneven_rows)
{
    SpMV::SparseMatrix_JDS<T> matrix(3, 3);

    // A = [1 0 2]
    //     [0 3 0]
    //     [4 5 6]
    // Insert only the nonzero entries.
    matrix.setValue(0, 0, T{1});
    matrix.setValue(0, 2, T{2});
    matrix.setValue(1, 1, T{3});
    matrix.setValue(2, 0, T{4});
    matrix.setValue(2, 1, T{5});
    matrix.setValue(2, 2, T{6});

    // Convert the construction data into JDS storage.
    matrix.assemble();

    // Input vector x and the independently computed expected product A*x.
    const std::vector<T> input = {T{1}, T{2}, T{3}};
    const std::vector<T> expected = {T{7}, T{6}, T{32}};

    // Compute A*x using the assembled JDS matrix.
    const std::vector<T> actual = matrix.matvec(input);

    // Check the length before accessing output entries.
    ASSERT(actual.size() == expected.size());

    // Match the absolute tolerance used by the other JDS tests.
    const T tolerance = static_cast<T>(1e-3);

    // Check results in the original matrix row order.
    for (std::size_t row = 0; row < expected.size(); ++row)
    {
        ASSERT_NEAR(actual[row], expected[row], tolerance);
    }
}

// Check that an empty row produces a zero result.
template <typename T>
TEST_CASE(matvec_empty_row)
{
    SpMV::SparseMatrix_JDS<T> matrix(3, 3);

    // A = [1 0 2]
    //     [0 0 0]
    //     [4 5 6]
    // Leave row 1 empty.
    matrix.setValue(0, 0, T{1});
    matrix.setValue(0, 2, T{2});
    matrix.setValue(2, 0, T{4});
    matrix.setValue(2, 1, T{5});
    matrix.setValue(2, 2, T{6});

    matrix.assemble();

    // Input vector x and the independently computed expected product A*x.
    const std::vector<T> input = {T{1}, T{2}, T{3}};
    const std::vector<T> expected = {T{7}, T{0}, T{32}};
    const std::vector<T> actual = matrix.matvec(input);

    ASSERT(actual.size() == expected.size());

    // Match the absolute tolerance used by the other JDS tests.
    const T tolerance = static_cast<T>(1e-3);

    for (std::size_t row = 0; row < expected.size(); ++row)
    {
        ASSERT_NEAR(actual[row], expected[row], tolerance);
    }
}

// Run both cases with the selected floating-point type.
template <typename T>
TEST_SUITE(jds_matvec_suite)
{
    TEST(matvec_uneven_rows<T>);
    TEST(matvec_empty_row<T>);
}

auto main() -> int {
  RUN_SUITE(basic_suite<float>);
  RUN_SUITE(basic_suite<double>);
  RUN_SUITE(accessor_suite<float>);
  RUN_SUITE(accessor_suite<double>);
  RUN_SUITE(assemble_suite<float>);
  RUN_SUITE(assemble_suite<double>);
  RUN_SUITE(jds_matvec_suite<float>);
  RUN_SUITE(jds_matvec_suite<double>);
  return 0;
}