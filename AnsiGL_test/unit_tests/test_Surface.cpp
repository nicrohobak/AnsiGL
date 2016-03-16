/*
 * test_Surface.cpp
 *
 * This is a collection of unit tests against the classes in AnsiGL()
 */


#include <AnsiGL/surface.h>

#include "unit_test.h"


using namespace std;
using namespace AnsiGL;



bool test_Surface()
{
    cout << "Testing AnsiGL::Surface..." << endl;

    Surface::Ptr Test( new Surface() );

    ANNOUNCE( "AnsiGL::Surface", "Width( int )" );
    Test->Width( 80 );
    TEST( Test->Width() == 80 );

    ANNOUNCE( "", "Height( int )" );
    Test->Height( 25 );
    TEST( Test->Height() == 25 );

    ANNOUNCE( "", "HasPoint( Point )" );
    TEST( Test->HasPoint(Point(4,7)) && !Test->HasPoint(Point(3,100)) );

    ANNOUNCE( "", "DrawChar( achar )" );
    Test->Resize( Area(3,3) );
    Test->DrawChar( achar("^RR"), Point(0,0) );
    Test->DrawChar( achar("^RR"), Point(0,1) );
    Test->DrawChar( achar("^RR"), Point(0,2) );
    Test->DrawChar( achar("^GG"), Point(1,0) );
    Test->DrawChar( achar("^GG"), Point(1,1) );
    Test->DrawChar( achar("^GG"), Point(1,2) );
    Test->DrawChar( achar("^BB"), Point(2,0) );
    Test->DrawChar( achar("^BB"), Point(2,1) );
    Test->DrawChar( achar("^BB"), Point(2,2) );
    TEST( Test->GetPixel(Point2D(0,0)) == achar("^RR") && Test->GetPixel(Point2D(1,1)) == achar("^GG") && Test->GetPixel(Point2D(2,2)) == achar("^BB") );

    string Expected("\033[0;1;31mR\033[0;1;32mG\033[0;1;34mB\033[0m\r\n\033[0;1;31mR\033[0;1;32mG\033[0;1;34mB\033[0m\r\n\033[0;1;31mR\033[0;1;32mG\033[0;1;34mB\033[0m\r\n");

    if ( gVerbose )
    {
	cout << "Expected: " << endl << Expected;
	cout << "------------------" << endl;
	cout << "Actual: " << endl << Test->Render();
    }

    ANNOUNCE( "", "Render()" );
    TEST( !Expected.compare(Test->Render()) );

    Surface::Ptr Helper( new Surface(Area(3,3)) );

    ANNOUNCE( "", "RenderToSurface( Surface, Point, bool )" );
    Test->RenderToSurface( Helper );
    TEST( !Expected.compare(Helper->Render()) );

    // TODO: Add the Draw*() functions...

    cout << "All tests for AnsiGL::Surface completed successfully!" << endl;

    return true;
}




