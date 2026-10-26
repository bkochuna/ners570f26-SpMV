#include "SpMV.hpp"

#include <csignal>   
#include <cstdio>    
#include <string_view>
#include <utility>   
#include <vector>    

#include <sys/types.h> 
#include <sys/wait.h>  
#include <unistd.h>    

// Testing library required for testing (Always include this last!)
#include "unit_test_framework.hpp"

// Helpers

// The library reports misuse (e.g. calling assemble() in the wrong state) with
// assert(), which aborts the process. To test that error handling, we run the
// offending code in a child process and check that it dies with SIGABRT.
template <typename F>
static bool abortsWhenRun(F&& func)
{
    std::fflush(nullptr); // avoid duplicating buffered output in the child

    const pid_t pid = fork();
    ASSERT(pid >= 0);

    if (pid == 0) {
        // Child: silence the expected assertion message and the library's
        // chatty output, then run the code under test.
        static_cast<void>(std::freopen("/dev/null", "w", stdout));
        static_cast<void>(std::freopen("/dev/null", "w", stderr));
        func();
        _exit(0); // Returned normally => did NOT abort
    }

    int status = 0;
    const pid_t waited = waitpid(pid, &status, 0);
    ASSERT(waited == pid);
    return WIFSIGNALED(status) && (WTERMSIG(status) == SIGABRT);
}

// Test matrix (3 x 4), 5 non-zeros:
//     [ 1.5   0    0   -2.0 ]
//     [  0    0   4.0   0   ]
//     [  0   7.25  0   0.5  ]
static constexpr size_t NROWS = 3;
static constexpr size_t NCOLS = 4;
static constexpr size_t NNZ   = 5;

template <typename T>
static std::vector<std::vector<T>> expectedDense()
{
    return {
        {static_cast<T>(1.5), static_cast<T>(0.0),  static_cast<T>(0.0),  static_cast<T>(-2.0)},
        {static_cast<T>(0.0), static_cast<T>(0.0),  static_cast<T>(4.0),  static_cast<T>(0.0)},
        {static_cast<T>(0.0), static_cast<T>(7.25), static_cast<T>(0.0),  static_cast<T>(0.5)}
    };
}

	template <typename T>
static void fillTestMatrix(SpMV::SparseMatrix_DEN<T>& A)
{
	A.setValue(0, 0, static_cast<T>(1.5));
	A.setValue(0, 3, static_cast<T>(-2.0));
	A.setValue(1, 2, static_cast<T>(4.0));
	A.setValue(2, 1, static_cast<T>(7.25));
	A.setValue(2, 3, static_cast<T>(0.5));
}

	template <typename T>
static T tol()
{
	return static_cast<T>(1e-6);
}

// Existing tests

	template <typename T>
TEST_CASE(test_DEN_class)
{
	SpMV::SparseMatrix_DEN<T> A(2, 3);

	// Verify the number of rows and columns.
	ASSERT(A.getNrows() == 2);
	ASSERT(A.getNcols() == 3);

	// Verify the initial matrix state.
	ASSERT(A.getState() == SpMV::MatrixState::initialized);
}

	template <typename T>
TEST_CASE(test_DEN_format)
{
	SpMV::SparseMatrix_DEN<T> A(NROWS, NCOLS);

	ASSERT(A.getFormat() == SpMV::MatrixType::DEN);
	ASSERT(A.printFormat() == std::string_view("DEN"));
}

// Activity 1a: assemble() tests

// Error handling: assemble() is only legal in the "building" state.
	template <typename T>
TEST_CASE(test_DEN_assemble_error_handling)
{
	// Control: assemble() on a properly built matrix must NOT abort, so we know
	// the aborts below are caused by the state check and nothing else.
	ASSERT(!abortsWhenRun([] {
				SpMV::SparseMatrix_DEN<T> A(NROWS, NCOLS);
				fillTestMatrix(A);
				A.assemble();
				}));

	// No values were ever set: matrix is still "initialized", not "building".
	ASSERT(abortsWhenRun([] {
				SpMV::SparseMatrix_DEN<T> A(NROWS, NCOLS);
				A.assemble();
				}));

	// Already assembled: assembling a second time is an error.
	ASSERT(abortsWhenRun([] {
				SpMV::SparseMatrix_DEN<T> A(NROWS, NCOLS);
				fillTestMatrix(A);
				A.assemble();
				A.assemble();
				}));
}

// Initialization: assemble() must create a nrows x ncols dense array that is
// zero-filled everywhere that was not set, and move the matrix to "assembled".
	template <typename T>
TEST_CASE(test_DEN_assemble_initialization)
{
	SpMV::SparseMatrix_DEN<T> A(NROWS, NCOLS);

	// Nothing is allocated before assembly.
	ASSERT(A._a.empty());

	// A single entry is enough to enter the "building" state.
	A.setValue(1, 2, static_cast<T>(4.0));
	ASSERT(A.getState() == SpMV::MatrixState::building);

	A.assemble();

	ASSERT(A.getState() == SpMV::MatrixState::assembled);
	ASSERT(A.getNrows() == NROWS);
	ASSERT(A.getNcols() == NCOLS);
	ASSERT(A.getNumnz() == 1);

	// Dense storage has the right shape ...
	ASSERT(A._a.size() == NROWS);
	for (size_t i = 0; i < NROWS; ++i) {
		ASSERT(A._a[i].size() == NCOLS);
	}

	// ... and every entry except (1,2) is zero.
	for (size_t i = 0; i < NROWS; ++i) {
		for (size_t j = 0; j < NCOLS; ++j) {
			const T expected = (i == 1 && j == 2) ? static_cast<T>(4.0) : static_cast<T>(0.0);
			ASSERT_NEAR(A._a[i][j], expected, tol<T>());
		}
	}
}

// Proper assembly from given input: every set value lands at the right (i,j),
// including the first/last row and column, and a repeated setValue overwrites.
	template <typename T>
TEST_CASE(test_DEN_assemble_from_input)
{
	SpMV::SparseMatrix_DEN<T> A(NROWS, NCOLS);
	fillTestMatrix(A);

	// Overwrite an existing entry; the last value written must win and must
	// not add a new non-zero.
	A.setValue(1, 2, static_cast<T>(99.0));
	A.setValue(1, 2, static_cast<T>(4.0));
	ASSERT(A.getNumnz() == NNZ);

	A.assemble();

	ASSERT(A.getState() == SpMV::MatrixState::assembled);
	ASSERT(A.getNumnz() == NNZ);

	const auto expected = expectedDense<T>();
	ASSERT(A._a.size() == NROWS);
	for (size_t i = 0; i < NROWS; ++i) {
		ASSERT(A._a[i].size() == NCOLS);
		for (size_t j = 0; j < NCOLS; ++j) {
			ASSERT_NEAR(A._a[i][j], expected[i][j], tol<T>());
		}
	}
}

// Activity 1b: disassemble() tests

// Error handling: disassemble() is only legal in the "assembled" state.
	template <typename T>
TEST_CASE(test_DEN_disassemble_error_handling)
{
	// Control: disassemble() on an assembled matrix must NOT abort.
	ASSERT(!abortsWhenRun([] {
				SpMV::SparseMatrix_DEN<T> A(NROWS, NCOLS);
				fillTestMatrix(A);
				A.assemble();
				A.disassemble();
				}));

	// Freshly constructed ("initialized"): nothing to disassemble.
	ASSERT(abortsWhenRun([] {
				SpMV::SparseMatrix_DEN<T> A(NROWS, NCOLS);
				A.disassemble();
				}));

	// Still "building": never assembled.
	ASSERT(abortsWhenRun([] {
				SpMV::SparseMatrix_DEN<T> A(NROWS, NCOLS);
				fillTestMatrix(A);
				A.disassemble();
				}));

	// Already disassembled: matrix is back in "building".
	ASSERT(abortsWhenRun([] {
				SpMV::SparseMatrix_DEN<T> A(NROWS, NCOLS);
				fillTestMatrix(A);
				A.assemble();
				A.disassemble();
				A.disassemble();
				}));
}

// Proper disassembly: the build map gets back every value that was assembled
// and the matrix returns to the "building" state.
	template <typename T>
TEST_CASE(test_DEN_disassemble_proper)
{
	SpMV::SparseMatrix_DEN<T> A(NROWS, NCOLS);
	fillTestMatrix(A);
	A.assemble();
	ASSERT(A.getState() == SpMV::MatrixState::assembled);

	A.disassemble();

	ASSERT(A.getState() == SpMV::MatrixState::building);
	ASSERT(A.getNrows() == NROWS);
	ASSERT(A.getNcols() == NCOLS);

	// Every original non-zero is present in the build map with its value.
	const auto expected = expectedDense<T>();
	const auto& coeffs = A.getCoeffMap();
	for (size_t i = 0; i < NROWS; ++i) {
		for (size_t j = 0; j < NCOLS; ++j) {
			const auto it = coeffs.find(std::make_pair(i, j));
			if (it != coeffs.end()) {
				ASSERT_NEAR(it->second, expected[i][j], tol<T>());
			} else {
				// Not stored => must be an implicit zero.
				ASSERT_NEAR(expected[i][j], static_cast<T>(0.0), tol<T>());
			}
		}
	}
	ASSERT(coeffs.count(std::make_pair<size_t, size_t>(0, 0)) == 1);
	ASSERT(coeffs.count(std::make_pair<size_t, size_t>(0, 3)) == 1);
	ASSERT(coeffs.count(std::make_pair<size_t, size_t>(1, 2)) == 1);
	ASSERT(coeffs.count(std::make_pair<size_t, size_t>(2, 1)) == 1);
	ASSERT(coeffs.count(std::make_pair<size_t, size_t>(2, 3)) == 1);
}

// The build map should only hold the non-zeros (a sparse matrix), and
// getNumnz() should still agree with it after a disassemble.
	template <typename T>
TEST_CASE(test_DEN_disassemble_keeps_only_nonzeros)
{
	SpMV::SparseMatrix_DEN<T> A(NROWS, NCOLS);
	fillTestMatrix(A);
	A.assemble();
	A.disassemble();

	ASSERT(A.getNumnz() == NNZ);
	ASSERT(A.getCoeffMap().size() == NNZ);
}

// Disassemble -> edit -> assemble round trip.
	template <typename T>
TEST_CASE(test_DEN_disassemble_roundtrip)
{
	SpMV::SparseMatrix_DEN<T> A(NROWS, NCOLS);
	fillTestMatrix(A);
	A.assemble();
	A.disassemble();

	// Edit an existing entry and add a new one while in the building state.
	A.setValue(0, 0, static_cast<T>(-1.0));
	A.setValue(1, 0, static_cast<T>(3.0));
	A.assemble();

	ASSERT(A.getState() == SpMV::MatrixState::assembled);

	auto expected = expectedDense<T>();
	expected[0][0] = static_cast<T>(-1.0);
	expected[1][0] = static_cast<T>(3.0);

	for (size_t i = 0; i < NROWS; ++i) {
		for (size_t j = 0; j < NCOLS; ++j) {
			ASSERT_NEAR(A._a[i][j], expected[i][j], tol<T>());
		}
	}
}

// -----------------------------------------------------------------------------
// Suites
// -----------------------------------------------------------------------------

	template <typename T>
TEST_SUITE(DEN_suite)
{
	TEST(test_DEN_class<T>);
	TEST(test_DEN_format<T>);
}

	template <typename T>
TEST_SUITE(DEN_assemble_suite)
{
	TEST(test_DEN_assemble_error_handling<T>);
	TEST(test_DEN_assemble_initialization<T>);
	TEST(test_DEN_assemble_from_input<T>);
}

	template <typename T>
TEST_SUITE(DEN_disassemble_suite)
{
	TEST(test_DEN_disassemble_error_handling<T>);
	TEST(test_DEN_disassemble_proper<T>);
	TEST(test_DEN_disassemble_keeps_only_nonzeros<T>);
	TEST(test_DEN_disassemble_roundtrip<T>);
}

auto main() -> int
{
	RUN_SUITE(DEN_suite<double>);
	RUN_SUITE(DEN_assemble_suite<double>);
	RUN_SUITE(DEN_disassemble_suite<double>);

	RUN_SUITE(DEN_suite<float>);
	RUN_SUITE(DEN_assemble_suite<float>);
	RUN_SUITE(DEN_disassemble_suite<float>);
	return 0;
}
