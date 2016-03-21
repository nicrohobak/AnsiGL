/*
 * test_ustring.cpp
 *
 * This is a collection of unit tests against the classes in AnsiGL()
 */


#include <AnsiGL/string.h>
#include <AnsiGL/uchar.h>

#include "unit_test.h"


using namespace std;
using namespace AnsiGL;


bool test_ustring()
{
	cout << "Testing AnsiGL::ustring..." << endl;

	ustring Test;
	string StrHelper("This is a string with ¥ Unicode! ¥ in it.");

	ANNOUNCE( "AnsiGL::ustring", "operator=( const string & )" );
	Test = StrHelper;
	// No direct test for this...the next test covers it though
	cout << "Passed." << endl;

	ANNOUNCE( "", "Render() const" );
	TEST( !StrHelper.compare( Test.Render() ) );

	ANNOUNCE( "", "length() const" );
	TEST( Test.length() == 41 );

	ustring UstrHelper(" plus ¥ more! ¥");

	ANNOUNCE( "", "operator+=( const ustring & )" );
	Test = "Test";
	Test += UstrHelper;
	StrHelper = "Test plus ¥ more! ¥";
	TEST( !StrHelper.compare( Test.Render() ) );

	ANNOUNCE( "", "operator+=( const string & )" );
	StrHelper = "Test plus ¥ more! ¥ and more! ¥";
	Test += " and more! ¥";
	TEST( !StrHelper.compare( Test.Render() ) );

	ANNOUNCE( "", "operator==( const ustring & ) const" );
	Test = "Pretty much the same";
	UstrHelper = "Prett¥ much the same";
	bool opEqUstr_Result1 = Test != UstrHelper;
	Test[5] = "¥";
	TEST( opEqUstr_Result1 && Test == UstrHelper );

	ANNOUNCE( "", "operator==( const string & ) const" );
	Test = "Pretty much the same";
	StrHelper = "Prett¥ much the same";
	bool opEqStr_Result1 = Test != StrHelper;
	Test[5] = "¥";
	TEST( opEqStr_Result1 && Test == StrHelper );

	ANNOUNCE( "", "push_back( uchar )" );
	Test = "Test";
	Test.push_back( uchar("£") );
	StrHelper = "Test£";
	TEST( !StrHelper.compare( Test.Render() ) );

	ANNOUNCE( "", "push_back( achar )" );
	Test = "Test";
	Test.push_back( uchar("l") );
	StrHelper = "Testl";
	TEST( !StrHelper.compare( Test.Render() ) );

	ANNOUNCE( "", "push_back( char )" );
	Test = "Test";
	Test.push_back( uchar("l") );
	StrHelper = "Testl";
	TEST( !StrHelper.compare( Test.Render() ) );

	cout << "All tests for AnsiGL::ustring completed successfully!" << endl;

	return true;
}


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --indent-switches --pad-oper


