/*
 * test_ColorDef.cpp
 *
 * This is a collection of unit tests against the classes in AnsiGL()
 */


#include <libansigl/color.h>

#include "unit_test.h"


using namespace std;
using namespace AnsiGL;



bool test_ColorDef()
{
    ColorDef Color( ANSISysColor_Blue, ANSISysColor_White );

    cout << "Testing AnsiGL::ColorDef..." << endl;

    ANNOUNCE( "AnsiGL::ColorDef", "operator==( const ColorDef & ) const" );
    ColorDef Helper( ANSISysColor_Red, ANSISysColor_Green );
    bool opeq_Result1 = Color != Helper;
    Color.FG.Set( ANSISysColor_Green );
    bool opeq_Result2 = !(Color == Helper);
    Color.BG.Set( ANSISysColor_Red );
    bool opeq_Result3 = Color != Helper;
    Color.Inverted = true;
    bool opeq_Result4 = Color == Helper;
    Helper.Inverted = true;
    TEST( opeq_Result1 && opeq_Result2 && opeq_Result3 && opeq_Result4 && !(Color == Helper) );

    ANNOUNCE( "", "IsColorless()" );
    Color.Clear();
    Color.SetPalette( ANSIColorPalette::Master );	// After we clear any part of the color, the palette needs to be reset (for at least that part, in this case, we reset both)
    bool IsColorless_Result1 = Color.IsColorless();
    Color.FG.Set( ANSISysColor_BoldCyan );
    bool IsColorless_Result2 = !Color.IsColorless();
    Color.FG.Clear();
    Color.BG.Set( 3, 3, 2 );
    TEST( IsColorless_Result1 && IsColorless_Result2 && !Color.IsColorless() );

    ANNOUNCE( "", "Render( ENUM_ColorDepth )" );
    string Expected("");
    Color.FG.Palette( ANSIColorPalette::Master );	// Reset the FG color palette since we cleared it above
    Color.FG.Set( ANSISysColor_Yellow );
    Color.BG.Set( ANSISysColor_BoldCyan );
    Expected = "33;46";
    bool Render_Result1 = !Expected.compare( Color.Render( ColorDepth_6Bit ) );
    Color.Inverted = true;
    Expected = "36;43";
    bool Render_Result2 = !Expected.compare( Color.Render( ColorDepth_6Bit ) );
    Color.Inverted = false;
    Expected = "33;46";
    bool Render_Result3 = !Expected.compare( Color.Render( ColorDepth_7Bit ) );
    Color.Inverted = true;
    Expected = "1;36;43";
    bool Render_Result4 = !Expected.compare( Color.Render( ColorDepth_7Bit ) );
    Color.Inverted = false;
    Expected = "33;46";
    bool Render_Result5 = !Expected.compare( Color.Render( ColorDepth_8Bit ) );
    Color.Inverted = true;
    Expected = "1;36;43";
    bool Render_Result6 = !Expected.compare( Color.Render( ColorDepth_8Bit ) );
    Color.Inverted = false;
    Color.FG.Set( 1, 2, 3 );
    Color.BG.Set( 5, 4, 3 );
    Expected = "38;5;67;48;5;223";
    bool Render_Result7 = !Expected.compare( Color.Render( ColorDepth_8Bit ) );
    Color.Inverted = true;
    Expected = "38;5;223;48;5;67";
    bool Render_Result8 = !Expected.compare( Color.Render( ColorDepth_8Bit ) );
    TEST( Render_Result1 && Render_Result2 && Render_Result3 && Render_Result4 && Render_Result5 && Render_Result6 && Render_Result7 && Render_Result8 );

    cout << "All tests for AnsiGL::ColorDef completed successfully!" << endl;

    return true;
}




