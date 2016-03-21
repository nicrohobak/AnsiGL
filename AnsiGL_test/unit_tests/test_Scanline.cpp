/*
 * test_Scanline.cpp
 *
 * This is a collection of unit tests against the classes in AnsiGL()
 */


#include <AnsiGL/surface.h>

#include "unit_test.h"


using namespace std;
using namespace AnsiGL;


bool test_Scanline()
{
	Scanline Test;

	cout << "Testing AnsiGL::Scanline..." << endl;

	// Nothing really to be done...AnsiGL::Surface will have most of the tests

	cout << "All tests for AnsiGL::Scanline completed successfully!" << endl;

	return true;
}


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --indent-switches --pad-oper


