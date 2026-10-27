#include "SpMV.hpp"

#include <cstddef>  // std::size_t
#include <iostream> // std::cout
#include <sstream>  // std::ostringstream
#include <string>   // std::string
#include <vector>   // std::vector
#include <cstdio>   // std::remove
#include <fstream>  // std::ifstream

// Testing library required for testing to be included last
#include "unit_test_framework.hpp"

using namespace std;

// Write the 3 matrices I will be testing so 
// I only have to write them one time 
// to work for all 6 tests
//
// To write nonzero entries in each matrix
struct Entry { 
	std::size_t row; 
	std::size_t col; 
	double val; 
};

// 3x3 Identity matrix: 
struct Identity3 {
	static constexpr size_t nrows = 3, ncols = 3;
	static std::vector<Entry> entries() { 
		return {
			{0,0,1.0}, 
			{1,1,1.0}, 
			{2,2,1.0}
		}; 
	}
};

// Non-symmetric 4x4 matrix
struct NonSym4 {
	static constexpr size_t nrows = 4, ncols = 4;
	static std::vector<Entry> entries() {
		return {
			{0,1,2.0}, {0,2,1.4}, {0,3,-2.0}, // Row 0
						// Row 1 left empty for shiggles
			{2,1,-2.4}, // Row 2
			{3,3,2.5} 	// Row 3
		};
	}
};

// Empty Matrix
struct Zero3 {
	static constexpr size_t nrows = 3, ncols = 3;
    static std::vector<Entry> entries() { 
        return {}; 
    }
};

// Helper to build and assemble matrix from these structs.
template <class M, typename T>
SpMV::SparseMatrix_CSR<T> makeMatrix()
{
    SpMV::SparseMatrix_CSR<T> A(M::nrows, M::ncols);
    for (const Entry& e : M::entries())
    {
        A.setValue(e.row, e.col, static_cast<T>(e.val));
    }
    A.assemble();
    return A;
}

// Write each templated test:

// Helper function to read whole file into a string.
inline string readFile(const string& filename)
{
    ifstream in(filename);
    ASSERT(in.is_open());
    ostringstream contents;
    contents << in.rdbuf();
    return contents.str();
}

// Helper function to match expected text: 
template <class M, typename T>
string expectedViewText()
{
    ostringstream expected;
    expected << "CSR Matrix (" << M::nrows << " x " << M::ncols
             << "), nnz = " << M::entries().size() << "\n";
    expected << "Row  Col  Value\n";
    for (const Entry& e : M::entries()) {
        expected << e.row << "    " << e.col << "    "
                 << static_cast<T>(e.val) << "\n";
    }
    return expected.str();
}

// Helper function to compare actual vs expected 
// print both if they are different
inline void checkText(const string& actual, const string& expected)
{
    if (actual != expected) {
        cout << "--- got:\n" << actual << "--- expected:\n" << expected;
    }
    ASSERT(actual == expected);
}

// Test printing to any stream we hand it.
template <class M, typename T>
TEST_CASE(view_to_stream) {
    auto A = makeMatrix<M, T>();

    ostringstream out;
    A.view(out);

    checkText(out.str(), expectedViewText<M, T>());
}

// Test printing to the screen
template <class M, typename T>
TEST_CASE(view_to_screen) {
    auto A = makeMatrix<M, T>();

    // Send cout into a string, call view(), put cout back.
    ostringstream captured;
    streambuf* original = cout.rdbuf(captured.rdbuf());
    A.view();
    cout.rdbuf(original);

    checkText(captured.str(), expectedViewText<M, T>());
}

// Test printing to a file.
template <class M, typename T>
TEST_CASE(view_to_file) {
    auto A = makeMatrix<M, T>();
    const string filename = "csr_view_test_output.txt";

    ASSERT(A.view(filename));              // must report success
    const string contents = readFile(filename);
    std::remove(filename.c_str());         // clean up before comparing

    checkText(contents, expectedViewText<M, T>());
}

// Test exporting to a matrix market file
template <class M, typename T>
TEST_CASE(view_matrix_market) {

	// TO DO:
	// write A to a temp file, read it back, and check:
    // line 1 == "%%MatrixMarket matrix coordinate real general"
    // line 2 == "nrows ncols nnz"
    // one line per entry: row+1  col+1  value   (1-based)

}

// Test exporting to a matlab-readable file
template <class M, typename T>
TEST_CASE(view_matlab) {

	// TO DO

}

// Test exporting to a .npy file
template <class M, typename T>
TEST_CASE(view_npy) {

	// TO DO

}

// Test that bad path reports an error instead of crashing.
template <class M, typename T>
TEST_CASE(view_invalid_path) {
    auto A = makeMatrix<M, T>();
    const string badPath = "no_such_directory/out.txt";

    ASSERT(!A.view(badPath));
}

// Run all checks for one matrix in a single Test Suite: 
template <class M, typename T>
TEST_SUITE(csr_view_suite) {
    TEST((view_to_screen<M, T>));
    TEST((view_to_stream<M, T>));
    TEST((view_to_file<M, T>));
    //TEST((view_matrix_market<M, T>));
    //TEST((view_matlab<M, T>));
    //TEST((view_npy<M, T>));
    TEST((view_invalid_path<M, T>));
}

// Run test suite for each of the 3 matrices and for both double and float types:
auto main() -> int {
	// test with double
    RUN_SUITE((csr_view_suite<Identity3, double>));
    RUN_SUITE((csr_view_suite<NonSym4,   double>));
    //RUN_SUITE((csr_view_suite<Zero3,     double>));
    
	// test with float
	RUN_SUITE((csr_view_suite<Identity3, float>));
    RUN_SUITE((csr_view_suite<NonSym4,   float>));
    //RUN_SUITE((csr_view_suite<Zero3,     float>));
    
	return 0;
}


