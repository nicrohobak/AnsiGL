/*
 * test_Pixel.cpp
 *
 * This is a collection of unit tests against the classes in AnsiGL()
 */


#include <AnsiGL/pixel.h>

#include "unit_test.h"


using namespace std;
using namespace AnsiGL;


bool test_Pixel()
{
	Pixel Test;

	cout << "Testing AnsiGL::Pixel..." << endl;

	// Nothing really to be done...AnsiGL::Surface will have most of the tests

	cout << "All tests for AnsiGL::Pixel completed successfully!" << endl;

	return true;
}


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --pad-oper


