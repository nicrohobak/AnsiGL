/*
 * test_ANSIColorPalette.cpp
 *
 * This is a collection of unit tests against the classes in AnsiGL()
 */


#include <AnsiGL/ansi.h>

#include "unit_test.h"


using namespace std;
using namespace AnsiGL;


bool test_ANSIColorPalette()
{
	cout << "Testing AnsiGL::ANSIColorPalette..." << endl;

	ANNOUNCE( "AnsiGL::ANSIColorPalette", "InitMasterPalette()" );
	ANSIColorPalette::InitMasterPalette();
	TEST( ANSIColorPalette::Master && ANSIColorPalette::Master->size() == 256 && (*ANSIColorPalette::Master)[247] );

	ANSIColorDef::Ptr Color;

	ANNOUNCE( "", "FindIndex( ANSIColorDef )" );
	TEST( ANSIColorPalette::Master->FindIndex( ANSIColorDef(ANSISysColor_Blue) ) == 4 );

	ANNOUNCE( "", "FindIndex( string )" );
	TEST( ANSIColorPalette::Master->FindIndex( "Bold Cyan" ) == 14 );

	ANNOUNCE( "", "FindIndex( ENUM_ANSISystemColors )" );
	TEST( ANSIColorPalette::Master->FindIndex( ANSISysColor_Yellow ) == 3 );

	ANNOUNCE( "", "FindIndex( unsigned char, unsigned char, unsigned char )" );
	TEST( ANSIColorPalette::Master->FindIndex( 2, 4, 3 ) == 115 );

	ANNOUNCE( "", "FindIndex( unsigned char )" );
	TEST( ANSIColorPalette::Master->FindIndex( 3 ) == 235 );

	ANNOUNCE( "", "FindColor( ENUM_ANSISystemColors )" );
	Color = ANSIColorPalette::Master->FindColor( ANSISysColor_BoldWhite );
	TEST( Color->Index() == 15 );

	ANNOUNCE( "", "FindIndex( string )" );
	TEST( ANSIColorPalette::Master->FindIndex( "White" ) == 7 );

	ANNOUNCE( "", "FindColor( unsigned char, unsigned char, unsigned char )" );
	Color = ANSIColorPalette::Master->FindColor( 5, 3, 1 );
	TEST( Color->Index() == 215 );

	ANNOUNCE( "", "FindColor( unsigned char )" );
	Color = ANSIColorPalette::Master->FindColor( 10 );
	TEST( Color->Index() == 242 );

	cout << "All tests for AnsiGL::ANSIColorPalette completed successfully!" << endl;

	return true;
}


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --pad-oper


