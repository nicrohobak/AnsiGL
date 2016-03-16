/*
 * test_Area.cpp
 *
 * This is a collection of unit tests against the classes in AnsiGL()
 */


#include <AnsiGL/area.h>

#include "unit_test.h"


using namespace std;
using namespace AnsiGL;


bool test_Area()
{
	cout << "Testing AnsiGL::Area..." << endl;

	Area Test;

	ANNOUNCE( "AnsiGL::Area", "Width( tSizeType )" );
	Test.Width( 5 );
	TEST( Test.Width() == 5 );

	ANNOUNCE( "", "Height( tSizeType )" );
	Test.Height( 9 );
	TEST( Test.Height() == 9 );

	Area Helper;
	Helper.Width( 4 );
	Helper.Height( 4 );

	ANNOUNCE( "", "ResizeTo( Area )" );
	Test.ResizeTo( Helper );
	TEST( Test.Width() == 4 && Test.Height() == 4 );

	ANNOUNCE( "", "ResizeTo( tSizeType, tSizeType )" );
	Test.ResizeTo( 50, 50 );
	TEST( Test.Width() == 50 && Test.Height() == 50 );

	ANNOUNCE( "", "operator==( const Area & )" );
	bool opIsEq_Result1 = !(Test == Helper);
	Test.ResizeTo( Helper );
	TEST( opIsEq_Result1 && Test == Helper );

	ANNOUNCE( "", "operator>( const Area & )" );
	Test.ResizeTo( 5, 5 );	// Area of 25
	Helper.ResizeTo( 5, 4 );	// Area of 20
	TEST( Test > Helper && !(Test < Helper) );

	Area Helper2( 4, 20 );

	ANNOUNCE( "", "operator>=( const Area & )" );
	Helper.ResizeTo( 5, 5 );	// Area of 25
	TEST( Test >= Helper && !(Helper2 <= Test) );

	cout << "All tests for AnsiGL::Area completed successfully!" << endl;

	return true;
}


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --pad-oper


