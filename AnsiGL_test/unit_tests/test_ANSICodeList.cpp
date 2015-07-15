/*
 * test_ANSICodeList.cpp
 *
 * This is a collection of unit tests against the classes in AnsiGL()
 */


#include <libansigl/ansi.h>

#include "unit_test.h"


using namespace std;
using namespace AnsiGL;



bool test_ANSICodeList()
{
    ANSICodeList List;

    cout << "Testing AnsiGL::ANSICodeList..." << endl;

    ANNOUNCE( "AnsiGL::ANSICodeList", "Add( ENUM_ANSICodes ) *" );
    List.Add( ANSI_FG_Black );
    TEST( !List.empty() );

    ANNOUNCE( "", "Has( ENUM_ANSICodes )" );
    TEST( !List.Has( ANSI_FG_Red ) && List.Has( ANSI_FG_Black ) );

    ANNOUNCE( "", "Remove( ENUM_ANSICodes )" );
    List.Remove( ANSI_FG_Black );
    TEST( !List.Has( ANSI_FG_Black ) );

    ANNOUNCE( "", "Add( ANSICodeList )" );
    ANSICodeList HelperList;
    HelperList.Add( ANSI_BoldOn );
    HelperList.Add( ANSI_ItalicsOn );
    HelperList.Add( ANSI_UnderlineOn );
    List.clear();
    List.Add( HelperList );
    TEST( List.size() == 3 && List.Has( ANSI_BoldOn ) && List.Has( ANSI_ItalicsOn ) && List.Has( ANSI_UnderlineOn ) );

    ANNOUNCE( "", "HasAny( ANSICodeList )" );
    HelperList.clear();
    HelperList.Add( ANSI_Default );
    HelperList.Add( ANSI_BoldOn );
    HelperList.Add( ANSI_NormalIntensity );
    bool HasAny_Result1 = List.HasAny( HelperList );
    HelperList.Remove( ANSI_BoldOn );
    TEST( HasAny_Result1 && !List.HasAny( HelperList ) );

    ANNOUNCE( "", "HasAll( ANSICodeList )" );
    bool HasAll_Result1 = !List.HasAll( HelperList );
    HelperList.clear();
    HelperList.Add( ANSI_BoldOn );
    HelperList.Add( ANSI_UnderlineOn );
    TEST( !HasAll_Result1 && List.HasAll( HelperList ) );

    ANNOUNCE( "", "Remove( ANSICodeList )" );
    List.Remove( HelperList );
    TEST( List.size() == 1 && List.Has( ANSI_ItalicsOn ) );

/*
    // This would be a tedious test to write completely...and it may be obsolete...so it is being held off on for now
    ANNOUNCE( "", "RemoveConflicts()" );
    TEST( false );
*/

    ANNOUNCE( "", "Render() *" );
    HelperList.clear();
    HelperList.Add( ANSI_Default );
    HelperList.Add( ANSI_BlinkSlow );
    HelperList.Add( ANSI_NormalIntensity );
    HelperList.Add( ANSI_BG_256Color );
    string ListExpected("3");
    string HelperListExpected("0;5;22;48");
    TEST( !ListExpected.compare(List.Render()) && !HelperListExpected.compare(HelperList.Render()) );

    cout << "All tests for AnsiGL::ANSICodeList completed successfully!" << endl;

    return true;
}




