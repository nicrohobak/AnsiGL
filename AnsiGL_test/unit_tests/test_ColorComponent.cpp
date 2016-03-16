/*
 * test_ColorComponent.cpp
 *
 * This is a collection of unit tests against the classes in AnsiGL()
 */


#include <AnsiGL/color.h>

#include "unit_test.h"


using namespace std;
using namespace AnsiGL;



bool test_ColorComponent()
{
    ColorComponent Component;

    cout << "Testing AnsiGL::ColorComponent..." << endl;

    ANNOUNCE( "AnsiGL::ColorComponent", "Set( ENUM_ANSISystemColors )" );
    Component.Set( ANSISysColor_BoldCyan );
    TEST( Component.Index() == (ANSI_FG_Cyan - 30) + 8 ); // Cyan (without the ANSI offset), plus the bold offset

    ANNOUNCE( "", "Set( unsigned char, unsigned char, unsigned char )" );
    Component.Set( 4, 1, 2 );
    TEST( Component.Index() == 168 );

    ANNOUNCE( "", "Set( unsigned int )" );
    Component.Set( 19 );
    TEST( Component.Index() == 251 );

    ANNOUNCE( "", "Color()" );
    ANSIColorDef::Ptr ANSIColor = Component.Color();
    TEST( ANSIColor && ANSIColor->Index() == 251 );

    ANNOUNCE( "", "operator==( const ColorComponent & ) const" );
    ColorComponent Helper( ANSISysColor_Yellow );
    bool opeq_Result1 = Component != Helper;
    Helper = Component;
    TEST( opeq_Result1 && Component == Helper );

    ANNOUNCE( "", "Render()" );
    string Expected("48;5;251");
    TEST( !Expected.compare( Component.Render( ColorDepth_8Bit, true ) ) );

    cout << "All tests for AnsiGL::ColorComponent completed successfully!" << endl;

    return true;
}




