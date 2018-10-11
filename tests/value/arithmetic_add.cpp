// arithmetic_add.cpp: functional tests for arithmetic add of values
//
// Copyright (C) 2017-2018 Stillwater Supercomputing, Inc.
//
// This file is part of the universal numbers project, which is released under an MIT Open Source license.

#include "common.hpp"
#include <algorithm>		// for std::max used in value module_add

// TODO: can you make value<> be a no-dependency class independent of posit?

// turn on tracing: kinda misleading that these flags carry the POSIT prefix
#define POSIT_VERBOSE_OUTPUT
#define POSIT_TRACE_ADD
#include "../../posit/exceptions.hpp"
#include "../../bitblock/bitblock.hpp"
#include "../../posit/value.hpp"
#include "../test_helpers.hpp"

// (sign, scale, fraction) representation with sbits in scale and fbits in fraction
template<size_t sbits, size_t fbits>
int ValidateValueAdd(std::string tag, bool bReportIndividualTestCases) {
	//constexpr size_t NR_OF_VALUES = (size_t(1) << (1 + scale + fbits));
	constexpr size_t abits = fbits + 4;
	int nrOfFailedTestCases = 0;
	sw::unum::value<fbits> a, b;
	sw::unum::value<abits+1> sum;

	// assume scale is a 2's complement representation and thus ranges from -2^(sbits-1) to 2^(sbits-1) - 1
	int scale_lb = -(int(1) << (sbits - 1));
	int scale_ub = (int(1) << (sbits - 1)) - 1;
	size_t max_fract = (size_t(1) << fbits);
	sw::unum::bitblock<fbits> afraction, bfraction;
	for (size_t sign = 0; sign < 2; ++sign) {
		for (int scale = scale_lb; scale < scale_ub; ++scale) {
			for (size_t afrac = 0; afrac < max_fract; ++afrac) {
				afraction = sw::unum::convert_to_bitblock<5>(afrac);
				a.set(sign == 1, scale, afraction, false, false);
				// std::cout << components(a) << std::endl;
				for (size_t sign = 0; sign < 2; ++sign) {
					for (int scale = scale_lb; scale < scale_ub; ++scale) {
						for (size_t bfrac = 0; bfrac < max_fract; ++bfrac) {
							bfraction = sw::unum::convert_to_bitblock<5>(bfrac);
							b.set(sign == 1, scale, bfraction, false, false);
							sw::unum::module_add<fbits, abits>(a, b, sum);
							std::cout << components(a) << " + " << components(b) << " = " << components(sum) << std::endl;
						}
					}
				}
			}
		}
	}

	return nrOfFailedTestCases;
}

#define MANUAL_TESTING 1
#define STRESS_TESTING 0

int main(int argc, char** argv)
try {
	using namespace std;
	using namespace sw::unum;

	bool bReportIndividualTestCases = true;
	int nrOfFailedTestCases = 0;

	// Arithmetic tests for value class
	cout << endl << "value addition arithmetic tests" << endl;
	cout << (bReportIndividualTestCases ? " " : "not ") << "reporting individual testcases" << endl;

#if MANUAL_TESTING

	// a posit<8,0> has 5 sbits, and 5 fbits. it needs 5 sbits to capture products
	// nrOfFailedTestCases += ReportTestResult(ValidateValueAdd<3, 5>("FAIL", bReportIndividualTestCases), "value<5,5>", "addition");
	value<5> a = 1;
	value<5> b = -3;
	value<10> sum;
	cout << components(a) << endl;
	cout << components(b) << endl;
	module_add<5,9>(a, b, sum);
	cout << components(sum) << endl;

#else
	// Note: increment/decrement depend on the 2's complement ordering of the posit encoding
	// This implies that this functionality is independent of the <nbits,es> configuration of the posit.
	// Otherwise stated, an enumeration of tests for different posit configurations is a bit superfluous.

	nrOfFailedTestCases += ReportTestResult(ValidateValueAdd<3,5>("FAIL", bReportIndividualTestCases), "value<3,5>", "addition");


#endif // MANUAL_TESTING



	return (nrOfFailedTestCases > 0 ? EXIT_FAILURE : EXIT_SUCCESS);
}
catch (char const* msg) {
	std::cerr << msg << std::endl;
	return EXIT_FAILURE;
}
catch (const std::runtime_error& err) {
	std::cerr << "Uncaught runtime exception: " << err.what() << std::endl;
	return EXIT_FAILURE;
}
catch (...) {
	std::cerr << "Caught unknown exception" << std::endl;
	return EXIT_FAILURE;
}
