#include "SpMV.hpp"
#include "SparseMatrix_ELL.hpp"

#include <vector>
#include <fstream>
#include <string>
#include <sstream>

// Testing library required for testing
#include "unit_test_framework.hpp"

// Test case to check if output file is created
TEST_CASE(ELL_createViewFile) {
	// Call the ELL matrix form to test on simple 3x3
	std::cout << "Testing with a 3x3 matrix!" << std::endl;
	std::string file_path = call_test_matrix<float>();

	// Check that the file exists and can be opened
	std::ifstream infile(file_path);

	// Assertion 
	if (!infile.is_open()) {
		std::cerr << "TEST FAILED: The file path does not exist -> " << file_path <<std::end;
		ASSERT(infile.is_open());
	}
	infile.close();
	std::remove(file_path);
}

// Check if output file is written in
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

	ASSERT(!output.empty() && "TEST FAILED: File is empty!")
}

// Check if output file is in Matrix Market format
// template <class fp_type>
TEST_CASE(ELL_viewMatrixMarketFormat) {
	// Call the ELL matrix form to test on simple 3x3
	std::cout << "Testing with a 3x3 matrix!" << std::endl;
	std::string file_path = call_test_matrix<float>();
	// SpMV::SparseMatrix_ELL<fp_type> matrix(3,3);

	// fs::path test_path = "ELL.view";
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

// Template for using fp_type
template <class fp_type>
std::string call_test_matrix() {
	SpMV::SparseMatrix_ELL<fp_type> matrix(3,3);
	matrix.view();
	return "viewfile.view";
}


// Test suite
TEST_SUITE(ELL_testViewSuite) {
	// Run unit test when suite is run
	TEST(ELL_createViewFile);
	TEST(ELL_writtenViewFile);
	TEST(ELL_viewMatrixMarketFormat);
}

auto
main() -> int
{
	// Run unit tests
	// If test fails, program prints failure info and returns 1
	RUN_SUITE(ELL_testViewSuite);
	return 0;
}
