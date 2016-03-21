#ifndef __ANSIGL_UNIT_TEST_H__
#define __ANSIGL_UNIT_TEST_H__

/*
 * unit_test.h
 *
 * This is a collection of unit tests against the classes in AnsiGL()
 */


#include <iostream>

#include <AnsiGL/ansi.h>


using namespace std;
using namespace AnsiGL;


////////////////////////////////////////////////////////////////
//
// Unit Tests
//
// Test functions should be created prior to any .cpp file work for a new class!
// There are a number of reasons to follow this guideline...if you're unsure of why, please ask someone about the benefits!
//
// Suggested Class/Test Function Roadmap:
//     1. Think about your problem, and plan as many designs in your head as needed until a reasonable solution is met (this is the "measure once" part of programming, if one were to borrow the "measure twice, cut once" addage from carpentry)
//     2. Create a header file that defines the class interface (this is the "measure twice" part of the addage), taking a more careful look at exact syntax of the components involved; rework as needed, and never hesitate to go immediately back to the planning stage if a big enough problem arises.  It is much, much better to catch problems before you go any further than this if possible (it saves lots of programming time in the long run!)
//     3. Create a test function (probably in this file, or one that looks similar) that will test your classes functions for completeness
//     4. Flesh out the class member functions until the tests all pass as expected
//     5. Check in the class and start using it with much more confidence than if you had not completed the above steps
//
// Test Function Requirements:
// - Test functions return true upon success, false upon failure.
// - All test functions test a single class
//     - Each function in the class should have a test designed around it
//     - Any failed test should result in a failure return for the entire class (all or nothing)
//     - Tests should be as brief as possible and should only test as small of a portion as possible
// - The "standard" method for creating tests goes as follows:
//     1. ANNOUNCE() the function being tested
//     2. Perform necessary code to generate a boolean test result
//     3. Use TEST() like you would use assert()...the value within must evaluate to truth or the function exits and reports and error
//     4. Place an asterisk '*' after any function name to denote a "dubious" test...a test that my provide a false-positive since self-verification wasn't possible.  Typically, any functions marked this way will be testeded to completion, but possibly not fully tested until the class test has completed.
// * At minimum, a test can be performed in 2 lines, an ANNOUNCE() followed immediately by a TEST()
// * Each ANNOUNCE() overwrites the previous announcement, so tests must be performed sequentially with this system
// * If ANNOUNCE() is called without one of the name parameters, it keeps the previous value
//
////////////////////////////////////////////////////////////////


#include <cstdio>
#include <iostream>
#include <string>


extern string gCurScopeName;
extern string gCurFuncName;
extern bool gVerbose;


extern void ANNOUNCE( string scopeName, string funcName );
extern bool FAIL( string message, ... );				// NOTE: Always returns 'false'


//
// This TEST() macro should be treated like an assert statement.  If the
//
#define TEST( test_condition )	if ( gVerbose && (test_condition) )						\
				{										\
				    cout << "Passed." << endl;							\
				    cout.flush();								\
				}										\
				else if ( !(test_condition) )							\
				{										\
				    return FAIL( "\n\r    *****  %s::%s failed!  *****\n\r\n\r",		\
						(!gCurScopeName.empty() ? (gCurScopeName.c_str()) : ("")),	\
						(!gCurFuncName.empty() ? (gCurFuncName.c_str()) : ("")) );	\
				}


#endif // __ANSIGL_UNIT_TEST_H__


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --indent-switches --pad-oper


