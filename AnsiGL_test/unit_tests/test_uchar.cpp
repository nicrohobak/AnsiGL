/*
 * test_uchar.cpp
 *
 * This is a collection of unit tests against the classes in AnsiGL()
 */


#include <AnsiGL/uchar.h>

#include "unit_test.h"


using namespace std;
using namespace AnsiGL;


bool test_uchar()
{
	uchar Ch;

	cout << "Testing AnsiGL::uchar..." << endl;

	ANNOUNCE( "AnsiGL::uchar", "Glyph( string ) *" );
	Ch.Glyph( "This is a regular string" );
	string Expected("T");
	bool Glyph_Result1 = !Expected.compare( Ch.Glyph() );

	Ch.Glyph( "¥¥¥¥¥¥¥¥¥" );
	Expected = "¥";
	TEST( Glyph_Result1 && !Expected.compare( Ch.Glyph() ) );

	uchar ucharHelper("¥");
	string stringHelper("S with some junk...just 'cause");
	unsigned char charHelper = 200;	// Something above 128 to verify that will fail as a single bit...

	ANNOUNCE( "AnsiGL::uchar", "operator=( const string & )" );
	Ch = stringHelper;
	Expected = "S";
	TEST( !Expected.compare( Ch.Glyph() ) );

	ANNOUNCE( "AnsiGL::uchar", "operator=( const char & )" );
	Ch = charHelper;
	bool opeq_Result1 = !Expected.compare( Ch.Glyph() );
	Ch = 'y';
	Expected = "y";
	TEST( opeq_Result1 && !Expected.compare( Ch.Glyph() ) );

	ANNOUNCE( "AnsiGL::uchar", "operator==( const uchar & )" );
	bool checkEqU_Result1 = !(Ch == ucharHelper);
	Ch = ucharHelper;
	TEST( checkEqU_Result1 && Ch == ucharHelper );

	ANNOUNCE( "AnsiGL::uchar", "operator!=( const uchar & )" );
	bool checkNotEqU_Result1 = !(Ch != ucharHelper);
	Ch = 'S';
	TEST( checkNotEqU_Result1 && Ch != ucharHelper );

	ANNOUNCE( "AnsiGL::uchar", "operator==( const string & )" );
	stringHelper = "S";
	bool checkEqS_Result1 = (Ch == stringHelper);
	Ch = 'r';
	TEST( checkEqS_Result1 && !(Ch == stringHelper) );

	ANNOUNCE( "AnsiGL::uchar", "operator!=( const string & )" );
	bool checkNotEqS_Result1 = Ch != stringHelper;
	Ch = 'S';
	TEST( checkNotEqS_Result1 && !(Ch != stringHelper) );

	ANNOUNCE( "AnsiGL::uchar", "operator==( const char & )" );
	charHelper = 'W';
	bool checkEqC_Result1 = !(Ch == charHelper);
	Ch = "W";
	TEST( checkEqC_Result1 && Ch == charHelper );

	ANNOUNCE( "AnsiGL::uchar", "operator!=( const char & )" );
	bool checkNotEqC_Result1 = !(Ch != charHelper );
	charHelper = '.';
	TEST( checkNotEqC_Result1 && Ch != charHelper );

	cout << "All tests for uchar completed successfully!" << endl;

	return true;
}


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --indent-switches --pad-oper


