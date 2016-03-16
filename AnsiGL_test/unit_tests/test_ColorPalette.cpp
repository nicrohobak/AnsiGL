/*
 * test_ColorPalette.cpp
 *
 * This is a collection of unit tests against the classes in AnsiGL()
 */


#include <AnsiGL/ansi.h>
#include <AnsiGL/color.h>

#include "unit_test.h"


using namespace std;
using namespace AnsiGL;


bool test_ColorPalette()
{
	cout << "Testing AnsiGL::ColorPalette..." << endl;

	// New create a color palette
	ColorPalette Palette;

	ANNOUNCE( "AnsiGL::ColorPalette", "Add( ColorDef::Ptr )" );
	ColorDef::Ptr NewColor = ColorDef::Ptr( new ColorDef(ANSISysColor_Red, ANSISysColor_Black) );
	Palette.Add( NewColor );
	NewColor = ColorDef::Ptr( new ColorDef(ANSISysColor_Yellow, ANSISysColor_Cyan) );
	Palette.Add( NewColor );
	NewColor = ColorDef::Ptr( new ColorDef(ANSISysColor_Red, ANSISysColor_Black) );
	Palette.Add( NewColor );
	NewColor = ColorDef::Ptr( new ColorDef(ANSISysColor_BoldBlue, ANSISysColor_White) );
	Palette.Add( NewColor );
	NewColor = ColorDef::Ptr( new ColorDef(ANSISysColor_Yellow, ANSISysColor_Cyan) );
	Palette.Add( NewColor );
	NewColor = ColorDef::Ptr( new ColorDef(ANSISysColor_Red, ANSISysColor_Black) );
	Palette.Add( NewColor );
	TEST( Palette.size() == 3 );


	if ( gVerbose )
	{
		for ( unsigned int i = 0; i < Palette.size(); ++i )
			cout << "Palette Index " << i << ": " << "\033[0;" << Palette[i]->Render() << "m" << "Test" << "\033[0m" << endl;
	}

	ColorDef TestColor( ANSISysColor_Yellow, ANSISysColor_Cyan );
	ColorDef::Ptr CurColor;

	ANNOUNCE( "", "operator[]( const int index ) const" );
	CurColor = Palette[1];
	bool opIndex_Result1 = (*CurColor) == TestColor;
	CurColor = Palette[0];
	TestColor.FG.Set( ANSISysColor_Red );
	TestColor.BG.Set( ANSISysColor_Black );
	TEST( opIndex_Result1 && (*CurColor) == TestColor );

	ANNOUNCE( "", "FindIndex( ColorDef color )" );
	bool FindIndex_Result1 = (Palette.FindIndex( TestColor ) == 0);
	TestColor.FG.Set( ANSISysColor_Yellow );
	TestColor.BG.Set( ANSISysColor_Cyan );
	bool FindIndex_Result2 = (Palette.FindIndex( TestColor ) == 1);
	TEST( FindIndex_Result1 && FindIndex_Result2 );

	ANNOUNCE( "", "InvertColors()" );
	Palette.InvertColors();
	TestColor.FG.Set( ANSISysColor_Cyan );
	TestColor.BG.Set( ANSISysColor_Yellow );
	TEST( Palette.FindIndex( TestColor ) );

	ANNOUNCE( "", "Remove( ColorDef color )" );
	Palette.Remove( TestColor );
	TestColor.FG.Set( ANSISysColor_Black );
	TestColor.BG.Set( ANSISysColor_Red );
	TEST( Palette.size() == 2 && Palette.FindIndex( TestColor ) == 0 );

	ANNOUNCE( "", "Remove( int index )" );
	Palette.Remove( 0 );
	TEST( Palette.size() == 1 );

	cout << "All tests for ColorPalette completed successfully!" << endl;

	return true;
}


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --pad-oper


