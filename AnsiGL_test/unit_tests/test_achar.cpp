/*
 * test_achar.cpp
 *
 * This is a collection of unit tests against the classes in AnsiGL()
 */


#include <AnsiGL/achar.h>

#include "unit_test.h"


using namespace std;
using namespace AnsiGL;



bool test_achar()
{
    achar Test;

    cout << "Testing AnsiGL::achar..." << endl;

    ANNOUNCE( "AnsiGL::achar", "operator=( const string & )" );
    Test = "^rTest"; // Should only get a red 'T'
    TEST( Test.Color.FG.Index() == (ANSI_FG_Red - ANSI_FG_Start) && Test.Glyph().length() == 1 );

    achar Helper("T");

    ANNOUNCE( "", "operator==( const achar & ) const" );
    bool opIsEqAchar_Result1 = Test != Helper;
    Helper = Test;
    TEST( opIsEqAchar_Result1 && Test == Helper );

    uchar UcharHelper;

    ANNOUNCE( "", "operator==( const uchar & ) const" );
    Test = "^y¥";
    UcharHelper = "¥";
    TEST( Test == UcharHelper );

    Test = Helper;

    ANNOUNCE( "", "HasAnsi()" );
    bool HasAnsi_Result1 = Test.HasAnsi();
    Test.Color.Clear();
    TEST( HasAnsi_Result1 && !Test.HasAnsi() );

    string Expected("");

    ANNOUNCE( "", "RenderAnsi( ENUM_ColorDepth )" );
    Test.Color.SetPalette( ANSIColorPalette::Master );	// Reset our palette
    Test.Color.FG.Set( ANSISysColor_Red );		// Set a color
    Expected = "\033[0;31m";
    bool RenderAnsi_Result1 = !Expected.compare( Test.RenderAnsi() );
    TEST( RenderAnsi_Result1 );

    ANNOUNCE( "", "Render( bool, bool, ENUM_ColorDepth )" );
    Test.Bell = true;
    Expected = "\007\033[0;31mT\033[0m";
    bool Render_Result1 = !Expected.compare( Test.Render(ColorDepth_Default, true, true) );
    Expected = "\033[0;31mT\033[0m";
    bool Render_Result2 = !Expected.compare( Test.Render(ColorDepth_Default, true, false) );
    Expected = "T";
    bool Render_Result3 = !Expected.compare( Test.Render(ColorDepth_Default, false, false) );
    TEST( Render_Result1 && Render_Result2 && Render_Result3 );

    cout << "All tests for AnsiGL::achar completed successfully!" << endl;

    return true;
}




