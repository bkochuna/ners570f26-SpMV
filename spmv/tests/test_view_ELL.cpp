#include "SpMV.hpp"
#include "SparseMatrix_ELL.hpp"

#include <iostream>
#include <vector>

// Testing library required for testing
#include "unit_test_framework.hpp"

// Test case to check if output file is created
template <class fp_type>
TEST_CASE(SparseMatrix_ELL_createViewFile) {
	// Call the ELL matrix form to test on simple 3x3
	SpMV::SparseMatrix_ELL<fp_type> matrix(3,3);

	fs::path test_path = "ELL.view";

	// Ensure file does not exist beforehand
	if (fs::exists(test_path)) {
		fs::remove(test_path);
	}

	// Generate matrix view
	matrix.view();

	// Assertion 
	ASSERT(fs::exists(test_path) && "TEST FAILED: The file was not created!");
	fs::remove(test_path);
}

// TODO: Test case to check if output file is in a specific format

// Test suite
TEST_SUITE(SparseMatrix_ELL_createViewFileviewELL_suite) {
	// Run unit test when suite is run
	TEST(viewELL);
}

auto
main() -> int
{
	// Run unit tests
	// If test fails, program prints failure info and returns 1
	RUN_SUITE(viewELL_suite);
	return 0;
}
