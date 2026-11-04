#include "SpMV.hpp"
#include "SparseMatrix.hpp"
#include "SparseMatrix_ELL.hpp"

#include <cstddef>
#include <vector>
#include <fstream> // Library used in view test cases
#include <sstream> // Library used in view test cases

// Testing library that's required for testing
#include "unit_test_framework.hpp"


// ============================================================================
// ==== BASIC TESTS: Implemented by leahghartman
// ============================================================================
// Test 1: Test initialization, including edge cases
template <typename T> TEST_CASE(test_initialization) {
    // Perform a standard initialization
    SpMV::SparseMatrix_ELL<T> test(3, 6);

    // Edge cases: no rows, no columns and the smallest non-empty matrix
    SpMV::SparseMatrix_ELL<T> test_0(0, 0);
    SpMV::SparseMatrix_ELL<T> test_1(1, 1);
}

// Test 2: Test that the format is reported as ELL
template <typename T> TEST_CASE(test_format) {
    // Initialize a matrix object and then assert that it is of the ELL type
    SpMV::SparseMatrix_ELL<T> test(2, 2);
    ASSERT(test.getFormat() == SpMV::MatrixType::ELL);
    ASSERT(test.printFormat() == "ELL");
}

// Test 3: Test that the constructor properly stores the dimensions, including
// for a non-square matrix where swapping rows and columns would be caught.
template <typename T> TEST_CASE(test_dim) {
    SpMV::SparseMatrix_ELL<T> test(6, 9);
    ASSERT(test.get_nrows() == 6);
    ASSERT(test.get_ncols() == 9);

    SpMV::SparseMatrix_ELL<T> test_0(0, 0);
    ASSERT(test_0.get_nrows() == 0);
    ASSERT(test_0.get_ncols() == 0);
}

// Test 4: Test that a newly constructed matrix holds no ELL data, since 
// nothing has been assembled yet.
template <typename T> TEST_CASE(test_initial_storage_empty) {
    SpMV::SparseMatrix_ELL<T> test(4, 4);
    ASSERT(test.get_max_length_row() == 0);
    ASSERT(test.get_col_idx().empty());
    ASSERT(test.get_values().empty());
}

// Basic test suite
template <typename T> TEST_SUITE(basic_suite) {
    TEST(test_initialization<T>);
    TEST(test_format<T>);
    TEST(test_dim<T>);
    TEST(test_initial_storage_empty<T>);
}

// NOTE: we can add everyone else's tests above this main definition, then call
// each suite inside the below main function.

// ============================================================================
// ==== Test Cases for Viewing: Implemented by zoujanice
// ============================================================================
// Template for using fp_type
template <class fp_type>
std::string call_test_matrix() {
	SpMV::SparseMatrix_ELL<fp_type> matrix(3,3);
	matrix.view();
	return "viewfile.view";
}

// Test case to check if output file is created
TEST_CASE(ELL_createViewFile) {
	// Call the ELL matrix form to test on simple 3x3
	std::cout << "Testing with a 3x3 matrix!" << std::endl;
	std::string file_path = call_test_matrix<float>();

	// Check that the file exists and can be opened
	std::ifstream infile(file_path);

	// Assertion 
	if (!infile.is_open()) {
		std::cerr << "TEST FAILED: The file path does not exist -> " << file_path << std::endl;
		ASSERT(infile.is_open());
	}
	infile.close();
	std::remove(file_path.c_str());
}

// Test case to check if output file is written in
TEST_CASE(ELL_writtenViewFile) {
	// Call the ELL matrix form to test on simple 3x3
	std::cout << "Testing with a 3x3 matrix!" << std::endl;
	std::string file_path = call_test_matrix<float>();

	// Check that file is available and open
	std::ifstream infile(file_path);
	ASSERT(infile.is_open() && "TEST FAILED: File cannot be opened!");

	// Check output
	std::stringstream buffer;
	buffer << infile.rdbuf();

	std::string output = buffer.str();

	ASSERT(!output.empty() && "TEST FAILED: File is empty!");
}

// Test case to check if output file is in Matrix Market format
TEST_CASE(ELL_viewMatrixMarketFormat) {
	// Call the ELL matrix form to test on simple 3x3
	std::cout << "Testing with a 3x3 matrix!" << std::endl;
	std::string file_path = call_test_matrix<float>();

	std::ifstream infile(file_path);

	// Check that file is available and open
	ASSERT(infile.is_open() && "TEST FAILED: File cannot be opened!");

	// Check output
	std::string header;
	ASSERT(static_cast<bool>(std::getline(infile,header)) && "TEST FAILED: Missing header!");

	std::istringstream header_stream(header);

	// Check for at least 3 numeric components
	size_t nrows, ncols, nnz;
	ASSERT(static_cast<bool>(header_stream >> nrows >> ncols >> nnz) && "TEST FAILED: Missing a component of the header! There should be 3 values: nrows ncols nnz");

	// Check for exactly 3 components
	std::string extra_comp;
	ASSERT(!(header_stream >> extra_comp) && "TEST FAILED: Header contains extra components! There should be 3 values: nrows ncols nnz");

	// Verify expected values with 3x3 test case
	if (nrows != 3 || ncols != 3) { //TODO: Check this! Include  || nnz != 0 only if constructor for ELL format returns a zero-matrix of givne dimension
		std::cerr << "TEST FAILED: Incorrect header values for rows and columns \n"
				  << "nrows = " << nrows << '\n'
				  << "cols  = " << ncols << '\n'
				  << "nnz   = " << nnz << '\n';
		ASSERT(nrows == 3 && ncols == 3); //TODO: Check this! Include && nnz == 0 only if constructor for ELL format returns a zero-matrix of given dimension
	}
}

// Test suite for view test cases
template <typename T> TEST_SUITE(ELL_testViewSuite) {
    // Run unit test when suite is run
	TEST(ELL_createViewFile);
	TEST(ELL_writtenViewFile);
	TEST(ELL_viewMatrixMarketFormat);
}
// ============================================================================


// ============================================================================
// ==== MAIN: Implemented by leahghartman
// ============================================================================
auto main() -> int {
    // Run all of the unit tests for each type
    RUN_SUITE(basic_suite<float>);
    RUN_SUITE(basic_suite<double>);
    return 0;
}

