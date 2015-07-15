/*
 * test_astring.cpp
 *
 * This is a collection of unit tests against the classes in AnsiGL()
 */


#include <libansigl/achar.h>
#include <libansigl/string.h>

#include "unit_test.h"


bool test_astring()
{
    std::cout << "Testing AnsiGL::astring..." << std::endl;

    AnsiGL::astring Test;
    AnsiGL::ustring UstrHelper("This is a ^{C5}test^D.");
    std::string StrHelper("This is a ^{y3}test^D.");

    ANNOUNCE( "AnsiGL::astring", "operator=( const ustring & )" );
    Test = UstrHelper;
    // No direct test for this...the next test covers it though
    std::cout << "Passed." << std::endl;

    std::string Expected("");

    ANNOUNCE( "", "Render() const" );
    Expected = "This is a \033[0;1;36;44mtest\033[0m.\033[0m";
    TEST( !Expected.compare(Test.Render()) );

    ANNOUNCE( "", "length() const" );
    TEST( Test.length() == 15 );	// The length WITHOUT any ANSI escape sequences is the appropriate length

    ANNOUNCE( "", "operator=( const string & )" );
    Test = StrHelper;
    Expected = "This is a \033[0;33;42mtest\033[0m.\033[0m";
    TEST( !Expected.compare(Test.Render()) );

    astring Helper("^BTEST^D.");

    ANNOUNCE( "", "operator+=( const astring & )" );
    Test = "This is another ";
    Test += Helper;
    Expected = "This is another \033[0;1;34mTEST\033[0m.\033[0m";
    TEST( !Expected.compare(Test.Render()) );

    ANNOUNCE( "", "operator+=( const ustring & )" );
    UstrHelper = "^B¥¥¥¥^D.";
    Test = "This is another ";
    Test += UstrHelper;
    Expected = "This is another \033[0;1;34m¥¥¥¥\033[0m.\033[0m";
    TEST( !Expected.compare(Test.Render()) );

    ANNOUNCE( "", "operator+=( const string & )" );
    StrHelper = "^Btest^D.";
    Test = "This is another ";
    Test += StrHelper;
    Expected = "This is another \033[0;1;34mtest\033[0m.\033[0m";
    TEST( !Expected.compare(Test.Render()) );

    ANNOUNCE( "", "operator==( const astring & ) const" );
    Test = "^rT^yE^cS^mT";
    Helper = "^RT^YE^CS^MT";
    bool opIsEqAstr_Result1 = Test != Helper;
    Test = Helper;
    TEST( opIsEqAstr_Result1 && Test == Helper );

    ANNOUNCE( "", "operator==( const ustring & ) const" );
    Test = "^rT^yE^cS^mT";
    UstrHelper = "TEST";
    bool opIsEqUstr_Result1 = Test != UstrHelper;
    UstrHelper = "^rT^yE^cS^mT";
    TEST( opIsEqUstr_Result1 && Test == UstrHelper );

    ANNOUNCE( "", "operator==( const string & ) const" );
    Test = "^rT^yE^cS^mT";
    StrHelper = "TEST";
    bool opIsEqStr_Result1 = Test != StrHelper;
    StrHelper = "^rT^yE^cS^mT";
    Test = "^rT^yE^cS^mT";
    TEST( opIsEqStr_Result1 && Test == StrHelper );

    ANNOUNCE( "", "push_back( achar ach )" );
    Test.push_back( achar("^w!") );
    Expected = "\033[0;31mT\033[0;33mE\033[0;36mS\033[0;35mT\033[0;37m!\033[0m";
    TEST( !Expected.compare(Test.Render()) );

    ANNOUNCE( "", "push_back( uchar uch )" );
    Test.push_back( uchar("£") );
    Expected = "\033[0;31mT\033[0;33mE\033[0;36mS\033[0;35mT\033[0;37m!\033[0m£\033[0m";
    TEST( !Expected.compare(Test.Render()) );

    ANNOUNCE( "", "push_back( char ch )" );
    Test.push_back( '@' );
    Expected = "\033[0;31mT\033[0;33mE\033[0;36mS\033[0;35mT\033[0;37m!\033[0m£@\033[0m";
    TEST( !Expected.compare(Test.Render()) );

    Test.clear();
    Test = "^rr^gg^bb ^RR^GG^BB";
    Expected = "\033[0;31mr\033[0;32mg\033[0;34mb \033[0;1;31mR\033[0;1;32mG\033[0;1;34mB\033[0m";

    if ( gVerbose )
    {
	std::cout << "Expected: " << Expected << std::endl;
	std::cout << "  Actual: " << Test.Render() << std::endl;
    }

    ANNOUNCE( "", "Render()" );
    TEST( !Expected.compare(Test.Render()) );

    std::cout << "All tests for AnsiGL::astring completed successfully!" << std::endl;

    return true;
}




