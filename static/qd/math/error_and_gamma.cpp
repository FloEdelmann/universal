// error_and_gamma.cpp: test suite runner for error and gamma functions for quad-double (qd) floating-point
//
// Copyright (C) 2017 Stillwater Supercomputing, Inc.
// SPDX-License-Identifier: MIT
//
// This file is part of the universal numbers project, which is released under an MIT Open Source license.
#include <universal/utility/directives.hpp>
#include <numbers>
#include <universal/number/qd/qd.hpp>
#include <universal/verification/test_suite.hpp>

// generate specific test case
template<typename Ty>
void GenerateTestCase(Ty fa) {
	unsigned precision = 25;
	unsigned width = 30;
	Ty fref;
	sw::universal::qd a, ref, v;
	a = fa;
	fref = std::erf(fa);
	ref = fref;
	v = sw::universal::erf(a);
	auto oldPrec = std::cout.precision();
	std::cout << std::setprecision(precision);
	std::cout << " -> erf(" << fa << ") = " << std::setw(width) << fref << std::endl;
	std::cout << " -> erf( " << a << ") = " << v << '\n' << to_binary(v) << '\n';
	std::cout << to_binary(ref) << "\n -> reference\n";
	std::cout << (ref == v ? "PASS" : "FAIL") << std::endl << std::endl;
	std::cout << std::setprecision(oldPrec);
}

// Regression testing guards: typically set by the cmake configuration, but MANUAL_TESTING is an override
#define MANUAL_TESTING 1
// REGRESSION_LEVEL_OVERRIDE is set by the cmake file to drive a specific regression intensity
// It is the responsibility of the regression test to organize the tests in a quartile progression.
//#undef REGRESSION_LEVEL_OVERRIDE
#ifndef REGRESSION_LEVEL_OVERRIDE
#undef REGRESSION_LEVEL_1
#undef REGRESSION_LEVEL_2
#undef REGRESSION_LEVEL_3
#undef REGRESSION_LEVEL_4
#define REGRESSION_LEVEL_1 1
#define REGRESSION_LEVEL_2 1
#define REGRESSION_LEVEL_3 1
#define REGRESSION_LEVEL_4 1
#endif

int main()
try {
	using namespace sw::universal;

	std::string test_suite  = "quad-double mathlib error/gamma function validation";
	std::string test_tag    = "error/gamma";
	bool reportTestCases    = false;
	int nrOfFailedTestCases = 0;

	ReportTestSuiteHeader(test_suite, reportTestCases);

#if MANUAL_TESTING

	GenerateTestCase(1.0);

	qd x(-3.0), increment(0.5);
	for (int i = 0; i < 14; i++) {
		std::cout << " erf( " << x << ") = " << erf(x) << '\n';
		x += increment;
	}
	x = -3.0;
	for (int i = 0; i < 14; i++) {
		std::cout << " erfc( " << x << ") = " << erfc(x) << '\n';
		x += increment;
	}

	//nrOfFailedTestCases += ReportTestResult(VerifyLogFunction<qd>("Manual Testing", reportTestCases), "qd", test_tag);

	ReportTestSuiteResults(test_suite, nrOfFailedTestCases);
	return EXIT_SUCCESS;   // ignore errors
#else


	ReportTestSuiteResults(test_suite, nrOfFailedTestCases);
	return (nrOfFailedTestCases > 0 ? EXIT_FAILURE : EXIT_SUCCESS);

#endif  // MANUAL_TESTING
}
catch (char const* msg) {
	std::cerr << "Caught ad-hoc exception: " << msg << std::endl;
	return EXIT_FAILURE;
}
catch (const sw::universal::universal_arithmetic_exception& err) {
	std::cerr << "Caught unexpected universal arithmetic exception : " << err.what() << std::endl;
	return EXIT_FAILURE;
}
catch (const sw::universal::universal_internal_exception& err) {
	std::cerr << "Caught unexpected universal internal exception: " << err.what() << std::endl;
	return EXIT_FAILURE;
}
catch (const std::runtime_error& err) {
	std::cerr << "Caught runtime exception: " << err.what() << std::endl;
	return EXIT_FAILURE;
}
catch (...) {
	std::cerr << "Caught unknown exception" << std::endl;
	return EXIT_FAILURE;
}
