/*
 * main.cpp
 *
 * This is a collection of unit tests against the classes in AnsiGL()
 */


#include <cstdarg>
#include <iostream>

#include "unit_test.h"


//
// Test function declarations
//
extern bool test_uchar();

extern bool test_ANSICodeList();
extern bool test_ANSIColorDef();
extern bool test_ANSIColorPalette();

extern bool test_ColorComponent();
extern bool test_ColorDef();
extern bool test_ColorPalette();

extern bool test_achar();

extern bool test_ustring();
extern bool test_astring();

extern bool test_Point();
extern bool test_Area();
extern bool test_FixedArea();

extern bool test_Pixel();
extern bool test_Scanline();

extern bool test_Surface();


int main( int argc, char *argv[] )
{
	// If we have any arguments at all, then become verbose
	gVerbose = (argc > 1) ? true : false;

	std::cout << "Starting a unit test of all AnsiGL classes' functionality.  (This could take a few moments...)" << endl;

	if ( !test_uchar() )
		return 1;

	if ( !test_ANSICodeList() )
		return 1;

	if ( !test_ANSIColorDef() )
		return 1;

	if ( !test_ANSIColorPalette() )
		return 1;

	if ( !test_ColorComponent() )
		return 1;

	if ( !test_ColorDef() )
		return 1;

	if ( !test_ColorPalette() )
		return 1;

	if ( !test_achar() )
		return 1;

	if ( !test_ustring() )
		return 1;

	if ( !test_astring() )
		return 1;

	if ( !test_Point() )
		return 1;

	if ( !test_Area() )
		return 1;

	if ( !test_FixedArea() )
		return 1;

	if ( !test_Pixel() )
		return 1;

	if ( !test_Scanline() )
		return 1;

	if ( !test_Surface() )
		return 1;

	std::cout << "All AnsiGL unit tests completed successfully!" << std::endl;

	return 0;
}


string gCurScopeName("");
string gCurFuncName("");
bool gVerbose = false;


void ANNOUNCE( string scopeName, string funcName )
{
	if ( !scopeName.empty() )
		gCurScopeName = scopeName;

	if ( !funcName.empty() )
		gCurFuncName = funcName;

	if ( !gVerbose )
		return;

	std::cout << "Testing " << gCurScopeName << "::" << gCurFuncName << "...";
	std::cout.flush();
}

bool FAIL( string message, ... )
{
	if ( message.empty() )
		return false;

	va_list va;
	va_start( va, message );

	vfprintf( stderr, message.c_str(), va );
	fflush( stderr );

	va_end( va );

	return false;
}


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --pad-oper


