/*
 * test_Point.cpp
 *
 * This is a collection of unit tests against the classes in AnsiGL()
 */


#include <AnsiGL/point.h>

#include "unit_test.h"


using namespace std;
using namespace AnsiGL;


bool test_Point()
{
	cout << "Testing AnsiGL::Point..." << endl;

	Point Test( 0, 0 );
	Point Helper( 3, 5 );

	ANNOUNCE( "AnsiGL::Point", "operator+=( const Point & )" );
	Test += Helper;
	TEST( Test.X() == 3 && Test.Y() == 5 );

	ANNOUNCE( "AnsiGL::Point", "operator-=( const Point & )" );
	Helper.X( 1 );
	Helper.Y( 2 );
	Test -= Helper;
	TEST( Test.X() == 2 && Test.Y() == 3 );

	ANNOUNCE( "AnsiGL::Point", "operator+( const Point &, const Point & )" );
	Helper.X( 4 );
	Helper.Y( 4 );
	Point Result = Test + Helper;
	TEST( Result.X() == 6 && Result.Y() == 7 );

	ANNOUNCE( "AnsiGL::Point", "operator-( const Point &, const Point & )" );
	Helper.X( 10 );
	Helper.Y( 4 );
	Result = Test - Helper;
	TEST( Result.X() == -8 && Result.Y() == -1 );

	cout << "All tests for AnsiGL::Point completed successfully!" << endl;

	return true;
}


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --indent-switches --pad-oper


