/*
 * test_<ClassName>.cpp
 *
 * This is a collection of unit tests against the classes in AnsiGL()
 */


#include "unit_test.h"


using namespace std;
using namespace AnsiGL;



bool test_<ClassName>()
{
    cout << "Testing AnsiGL::<ClassName>..." << endl;

    ANNOUNCE( "AnsiGL::<ClassName>", "<MemberFunc>" );
    TEST( 0 ); // The condition given to TEST() must not be false, or the program will exit and report failure

    ANNOUNCE( "", "<MemberFunc>" );
    TEST( 0 );

    cout << "All tests for AnsiGL::<ClassName> completed successfully!" << endl;

    return true;
}




