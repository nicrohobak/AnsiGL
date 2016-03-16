/*
 * test_ANSIColorDef.cpp
 *
 * This is a collection of unit tests against the classes in AnsiGL()
 */


#include <AnsiGL/ansi.h>

#include "unit_test.h"


using namespace std;
using namespace AnsiGL;


bool test_ANSIColorDef()
{
	ANSIColorDef ANSIColor;

	cout << "Testing AnsiGL::ANSIColorDef..." << endl;

	ANNOUNCE( "AnsiGL::ANSIColorDef", "SystemColor( ENUM_ANSISystemColors )" );
	ANSIColor.SystemColor( ANSISysColor_BoldGreen );
	TEST( ANSIColor.SystemColor() == ANSISysColor_BoldGreen && ANSIColor.Index() <= 15 );

	ANNOUNCE( "", "IsSysColor()" );
	TEST( ANSIColor.IsSysColor() );

	ANNOUNCE( "", "SetRGB( unsigned char, unsigned char, unsigned char )" );
	ANSIColor.SetRGB( 2, 5, 3 );
	unsigned char r = 0, g = 0, b = 0;
	ANSIColor.GetRGB( &r, &g, &b );
	TEST( r == 2 && g == 5 && b == 3 && ANSIColor.Index() >= 16 && ANSIColor.Index() <= 231 );

	ANNOUNCE( "", "IsRGB()" );
	TEST( ANSIColor.IsRGB() && !ANSIColor.IsSysColor() );

	ANNOUNCE( "", "Grayscale( unsigned char )" );
	ANSIColor.Grayscale( 15 );
	TEST( ANSIColor.Grayscale() == 15 && ANSIColor.Index() >= 232 );

	ANNOUNCE( "", "IsGrayscale()" );
	TEST( ANSIColor.IsGrayscale() && !ANSIColor.IsRGB() && !ANSIColor.IsSysColor() );

	ANNOUNCE( "", "operator==( const ANSIColorDef & )" );
	ANSIColorDef Helper;
	Helper.SystemColor( ANSISysColor_Magenta );
	bool opeq_Result1 = !(ANSIColor == Helper);
	ANSIColor = Helper;
	TEST( opeq_Result1 && ANSIColor == Helper );

	ANNOUNCE( "", "Render( ENUM_ColorDepth, bool )" );
	bool Depth6Bit = false, Depth7Bit = false, Depth8Bit = false;
	string Expected("");

	ANSIColor.SystemColor( ANSISysColor_BoldMagenta );
	Expected = "35";
	Depth6Bit = !Expected.compare( ANSIColor.Render( ColorDepth_6Bit ) );

	Expected = "45";
	Depth6Bit = Depth6Bit && !Expected.compare( ANSIColor.Render( ColorDepth_6Bit, true ) );

	Expected = "1;35";
	Depth7Bit = !Expected.compare( ANSIColor.Render( ColorDepth_7Bit ) );
	Depth8Bit = !Expected.compare( ANSIColor.Render( ColorDepth_8Bit ) );	// If it can, it will automatically convert to the more compatible color style, even if set to 8-bit explicitly

	Expected = "45";
	Depth7Bit = Depth7Bit && !Expected.compare( ANSIColor.Render( ColorDepth_7Bit, true ) );
	Depth8Bit = Depth8Bit && !Expected.compare( ANSIColor.Render( ColorDepth_8Bit, true ) );

	ANSIColor.SetRGB( 3, 5, 3 );
	Expected = "38;5;157";
	Depth8Bit = Depth8Bit && !Expected.compare( ANSIColor.Render( ColorDepth_8Bit ) );

	ANSIColor.SetRGB( 5, 5, 5 );
	Expected = "37";
	Depth6Bit = Depth6Bit && !Expected.compare( ANSIColor.Render( ColorDepth_6Bit ) );

	Expected = "1;37";
	Depth7Bit = Depth7Bit && !Expected.compare( ANSIColor.Render( ColorDepth_7Bit ) );
	TEST( Depth6Bit && Depth7Bit && Depth8Bit );

	cout << "All tests for AnsiGL::ANSIColorDef completed successfully!" << endl;

	return true;
}


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --pad-oper


