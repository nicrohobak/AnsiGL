/*
 * achar.cpp
 *
 * Defines an ANSI-enabled "char".
 */


#include <cctype>
#include <sstream>

#include "AnsiGL/achar.h"
#include "AnsiGL/string.h"


namespace AnsiGL
{
	//
	// Global Variables
	//
	const tColorCode *ActiveColorCodes = DefaultColorCodes;

	const tColorCode DefaultColorCodes[ ColorCode_MAX ] =
	{
		'^',		// ColorCode_Escape
		'{',		// ColorCode_List_Start
		'}',		// ColorCode_List_End

		'#',		// ColorCode_RGB
		'$',		// ColorCode_Grayscale

		'&',		// ColorCode_Bell

		'D',		// ColorCode_Clear_All

		'k',		// ColorCode_Black
		'r',		// ColorCode_Red
		'g',		// ColorCode_Green
		'y',		// ColorCode_Yellow
		'b',		// ColorCode_Blue
		'm',		// ColorCode_Magenta
		'c',		// ColorCode_Cyan
		'w',		// ColorCode_White
		'd',		// ColorCode_Default

		'K',		// ColorCode_Bold_Black
		'R',		// ColorCode_Bold_Red
		'G',		// ColorCode_Bold_Green
		'Y',		// ColorCode_Bold_Yellow
		'B',		// ColorCode_Bold_Blue
		'M',		// ColorCode_Bold_Magenta
		'C',		// ColorCode_Bold_Cyan
		'W',		// ColorCode_Bold_White

		'1',		// ColorCode_BG_Black
		'2',		// ColorCode_BG_Red
		'3',		// ColorCode_BG_Green
		'4',		// ColorCode_BG_Yellow
		'5',		// ColorCode_BG_Blue
		'6',		// ColorCode_BG_Magenta
		'7',		// ColorCode_BG_Cyan
		'8',		// ColorCode_BG_White
		'0',		// ColorCode_BG_Default

		'*',		// ColorCode_Bold_On
		'.',		// ColorCode_Bold_Off
		'/',		// ColorCode_Italics_On
		'\\',		// ColorCode_Italics_Off
		'_',		// ColorCode_Underline_On
		'-',		// ColorCode_Underline_Off
		'%',		// ColorCode_CrossedOut_On
		'=',		// ColorCode_CrossedOut_Off
		':',		// ColorCode_Blink_On
		';',		// ColorCode_Blink_Off
		'I',		// ColorCode_Invert_On
		'i'			// ColorCode_Invert_Off
	};


	//
	// Global Functions
	//
	astring ConvertColorCodes( const ustring &str,
							   const tColorCode *colorCodes )
	{
		return ConvertColorCodes( str, achar(), colorCodes );
	}

	astring ConvertColorCodes( const ustring &str,
							   const achar &initialANSIState,
							   const tColorCode *colorCodes )
	{
		const tColorCode *CurColorCodes = NULL;
		astring ConvertedStr;

		if ( str.empty() )
			return ConvertedStr;

		// If codes are provided, use those...otherwise try the active set
		if ( colorCodes )
			CurColorCodes = colorCodes;
		else
			CurColorCodes = ActiveColorCodes;

		// If we still don't have any codes, we're forced to abandon ship
		if ( !CurColorCodes )
			return ConvertedStr;

		achar CurAChar, CurANSIState(initialANSIState);
		auto s_begin = str.begin(), s_end = str.end();

		for ( auto CurChar = s_begin; CurChar != s_end; ++CurChar )
		{
			char CheckChar = !(CurChar->Glyph().empty()) ? (CurChar->Glyph()[0]) : ('\0');

			// Check for ASCII control characters, etc.  Replace them with a ' ' (space) glyph...but not ANSI escape codes
			if ( CheckChar <= 32 && CheckChar != '\033' )
				CheckChar = 32;

			// Check to see if we have a color sequence starting...if so...
			if ( CheckChar == CurColorCodes[ColorCode_Escape] )
			{
				bool BreakLoop = true;	// Default is true...the list start character sets false if we need more than just one pass
				bool ListSequence = false;

				while ( CurChar != s_end )
				{
					++CurChar;		// Advance to the next character

					// Verify that we're okay to proceed
					if ( CurChar == s_end )
						break;

					// Update our CheckChar
					CheckChar = !((*CurChar).Glyph().empty()) ? ((*CurChar).Glyph()[0]) : ('\0');

					// Had to sacrifice the switch for the sake of flexibility...
					if ( CheckChar == CurColorCodes[ColorCode_Escape] )
					{
						if ( !ListSequence )
						{
							CurAChar = CurANSIState;
							CurAChar.Glyph( CurColorCodes[ColorCode_Escape] );
							ConvertedStr.push_back( CurAChar );
							CurAChar.Clear();
						}
					}
					else if ( CheckChar == CurColorCodes[ColorCode_List_Start] )
					{
						ListSequence = true;
						BreakLoop = false;
					}
					else if ( CheckChar == CurColorCodes[ColorCode_List_End] )
					{
						if ( ListSequence )
							BreakLoop = true;
					}
					else if ( CheckChar == CurColorCodes[ColorCode_Bell] )
					{
						CurANSIState.Bell = true;
					}
					else if ( CheckChar == CurColorCodes[ColorCode_RGB]
						   || CheckChar == CurColorCodes[ColorCode_Grayscale] )
					{
						bool IsRGB = CheckChar == CurColorCodes[ColorCode_RGB];

						// Check for foreground/background
						bool Background = false;

						std::string Digits("");

						// Read digits until the first non-digit, storing them in Digits
						for ( ++CurChar; CurChar != s_end; ++CurChar )
						{
							CheckChar = !((*CurChar).Glyph().empty()) ? ((*CurChar).Glyph()[0]) : ('\0');

							if ( !isdigit( CheckChar ) )
								break;

							Digits.push_back( CheckChar );
						}

						if ( CurChar == s_end )
							break;

						CheckChar = !((*CurChar).Glyph().empty()) ? ((*CurChar).Glyph()[0]) : ('\0');

						// If the first non-digit is F or B, mark this color as such, and if it is neither, assume it to be F
						switch ( toupper(CheckChar) )
						{
							case 'B':
								Background = true;
							case 'F':
								break;

							default:
								--CurChar;
								break;
						}

						// Decypher the color digits here
						if ( IsRGB )
						{
							// Make sure we have enough digits
							if ( Digits.length() < 3 )
								break;

							std::string::const_iterator CurDigit = Digits.begin();
							std::string Temp;

							// Read 3 digits, 0-5
							Temp.push_back( *CurDigit );
							unsigned char R = atoi( Temp.c_str() );
							Temp.clear();
							Temp.push_back( *(CurDigit + 1) );
							unsigned char G = atoi( Temp.c_str() );
							Temp.clear();
							Temp.push_back( *(CurDigit + 2) );
							unsigned char B = atoi( Temp.c_str() );

							if ( R > 5 )
								R = 5;
							if ( G > 5 )
								G = 5;
							if ( B > 5 )
								B = 5;

							if ( Background )
								CurANSIState.Color.BG.Set( R, G, B );
							else
								CurANSIState.Color.FG.Set( R, G, B );
						}
						else
						{
							unsigned char Grayscale = atoi( Digits.c_str() );

							if ( Grayscale > 23 )
								Grayscale = 23;

							if ( Background )
								CurANSIState.Color.BG.Set( Grayscale );
							else
								CurANSIState.Color.FG.Set( Grayscale );
						}

						break;
					}
					else if ( CheckChar == CurColorCodes[ColorCode_Clear_All] )
					{
						ANSIColorPalette::Ptr FGPalette = CurANSIState.Color.FG.Palette();
						ANSIColorPalette::Ptr BGPalette = CurANSIState.Color.BG.Palette();
						CurANSIState.Clear();
						CurANSIState.Color.FG.Palette( FGPalette );
						CurANSIState.Color.BG.Palette( BGPalette );
						break;
					}
					//
					// Normal colors
					//
					else if ( CheckChar == CurColorCodes[ColorCode_Black] )
						CurANSIState.Color.FG.Set( ANSISysColor_Black );
					else if ( CheckChar == CurColorCodes[ColorCode_Red] )
						CurANSIState.Color.FG.Set( ANSISysColor_Red );
					else if ( CheckChar == CurColorCodes[ColorCode_Green] )
						CurANSIState.Color.FG.Set( ANSISysColor_Green );
					else if ( CheckChar == CurColorCodes[ColorCode_Yellow] )
						CurANSIState.Color.FG.Set( ANSISysColor_Yellow );
					else if ( CheckChar == CurColorCodes[ColorCode_Blue] )
						CurANSIState.Color.FG.Set( ANSISysColor_Blue );
					else if ( CheckChar == CurColorCodes[ColorCode_Magenta] )
						CurANSIState.Color.FG.Set( ANSISysColor_Magenta );
					else if ( CheckChar == CurColorCodes[ColorCode_Cyan] )
						CurANSIState.Color.FG.Set( ANSISysColor_Cyan );
					else if ( CheckChar == CurColorCodes[ColorCode_White] )
						CurANSIState.Color.FG.Set( ANSISysColor_White);
					else if ( CheckChar == CurColorCodes[ColorCode_Default] )
						CurANSIState.Color.FG.Set( ANSISysColor_Default );
					//
					// Bold colors
					//
					else if ( CheckChar == CurColorCodes[ColorCode_Bold_Black] )
						CurANSIState.Color.FG.Set( ANSISysColor_BoldBlack );
					else if ( CheckChar == CurColorCodes[ColorCode_Bold_Red] )
						CurANSIState.Color.FG.Set( ANSISysColor_BoldRed );
					else if ( CheckChar == CurColorCodes[ColorCode_Bold_Green] )
						CurANSIState.Color.FG.Set( ANSISysColor_BoldGreen );
					else if ( CheckChar == CurColorCodes[ColorCode_Bold_Yellow] )
						CurANSIState.Color.FG.Set( ANSISysColor_BoldYellow );
					else if ( CheckChar == CurColorCodes[ColorCode_Bold_Blue] )
						CurANSIState.Color.FG.Set( ANSISysColor_BoldBlue );
					else if ( CheckChar == CurColorCodes[ColorCode_Bold_Magenta] )
						CurANSIState.Color.FG.Set( ANSISysColor_BoldMagenta );
					else if ( CheckChar == CurColorCodes[ColorCode_Bold_Cyan] )
						CurANSIState.Color.FG.Set( ANSISysColor_BoldCyan );
					else if ( CheckChar == CurColorCodes[ColorCode_Bold_White] )
						CurANSIState.Color.FG.Set( ANSISysColor_BoldWhite);
					//
					// Background colors
					//
					else if ( CheckChar == CurColorCodes[ColorCode_BG_Black] )
						CurANSIState.Color.BG.Set( ANSISysColor_Black );
					else if ( CheckChar == CurColorCodes[ColorCode_BG_Red] )
						CurANSIState.Color.BG.Set( ANSISysColor_Red );
					else if ( CheckChar == CurColorCodes[ColorCode_BG_Green] )
						CurANSIState.Color.BG.Set( ANSISysColor_Green );
					else if ( CheckChar == CurColorCodes[ColorCode_BG_Yellow] )
						CurANSIState.Color.BG.Set( ANSISysColor_Yellow );
					else if ( CheckChar == CurColorCodes[ColorCode_BG_Blue] )
						CurANSIState.Color.BG.Set( ANSISysColor_Blue );
					else if ( CheckChar == CurColorCodes[ColorCode_BG_Magenta] )
						CurANSIState.Color.BG.Set( ANSISysColor_Magenta );
					else if ( CheckChar == CurColorCodes[ColorCode_BG_Cyan] )
						CurANSIState.Color.BG.Set( ANSISysColor_Cyan );
					else if ( CheckChar == CurColorCodes[ColorCode_BG_White] )
						CurANSIState.Color.BG.Set( ANSISysColor_White);
					//
					// Attributes
					//
					else if ( CheckChar == CurColorCodes[ColorCode_Bold_On] )
					{
						CurANSIState.RemoveAttribute( ANSI_BoldOff );
						CurANSIState.AddAttribute( ANSI_BoldOn );
					}
					else if ( CheckChar == CurColorCodes[ColorCode_Bold_Off] )
					{
						CurANSIState.RemoveAttribute( ANSI_BoldOn );
						CurANSIState.AddAttribute( ANSI_BoldOff );
					}
					else if ( CheckChar == CurColorCodes[ColorCode_Italics_On] )
					{
						CurANSIState.RemoveAttribute( ANSI_ItalicsOff );
						CurANSIState.AddAttribute( ANSI_ItalicsOn );
					}
					else if ( CheckChar == CurColorCodes[ColorCode_Italics_Off] )
					{
						CurANSIState.RemoveAttribute( ANSI_ItalicsOn );
						CurANSIState.AddAttribute( ANSI_ItalicsOff );
					}
					else if ( CheckChar == CurColorCodes[ColorCode_Underline_On] )
					{
						CurANSIState.RemoveAttribute( ANSI_UnderlineOff );
						CurANSIState.AddAttribute( ANSI_UnderlineOn );
					}
					else if ( CheckChar == CurColorCodes[ColorCode_Underline_Off] )
					{
						CurANSIState.RemoveAttribute( ANSI_UnderlineOn );
						CurANSIState.AddAttribute( ANSI_UnderlineOff );
					}
					else if ( CheckChar == CurColorCodes[ColorCode_CrossedOut_On] )
					{
						CurANSIState.RemoveAttribute( ANSI_CrossedOutOff );
						CurANSIState.AddAttribute( ANSI_CrossedOut );
					}
					else if ( CheckChar == CurColorCodes[ColorCode_CrossedOut_Off] )
					{
						CurANSIState.RemoveAttribute( ANSI_CrossedOut );
						CurANSIState.AddAttribute( ANSI_CrossedOutOff );
					}
					else if ( CheckChar == CurColorCodes[ColorCode_Blink_On] )
					{
						CurANSIState.RemoveAttribute( ANSI_BlinkOff );
						CurANSIState.AddAttribute( ANSI_BlinkSlow );	// Usually the only one implemented anyway...
					}
					else if ( CheckChar == CurColorCodes[ColorCode_Blink_Off] )
					{
						CurANSIState.RemoveAttribute( ANSI_BlinkSlow );
						CurANSIState.AddAttribute( ANSI_BlinkOff );
					}
					else if ( CheckChar == CurColorCodes[ColorCode_Invert_On] )
					{
						CurANSIState.RemoveAttribute( ANSI_InverseOff );
						CurANSIState.AddAttribute( ANSI_InverseOn );
					}
					else if ( CheckChar == CurColorCodes[ColorCode_Invert_Off] )
					{
						CurANSIState.RemoveAttribute( ANSI_InverseOn );
						CurANSIState.AddAttribute( ANSI_InverseOff );
					}

					if ( BreakLoop )	// Check here to guarantee one run-through at least
						break;
				}

				// Now that we've got that color code, just continue and press on like normal
				continue;
			}

			CurAChar = CurANSIState;
			CurAChar.Glyph( *CurChar );
			ConvertedStr.push_back( CurAChar );
			CurAChar.Clear();

			CurANSIState.Bell = false;		// Clear the bell each time so it isn't painted more than we actually want
		}

		// If we're just a color sequence with no glyph, make our glyph a space
		if ( ConvertedStr.empty() )
		{
			CurAChar = CurANSIState;
			CurAChar.Glyph( ' ' );
			ConvertedStr.push_back( CurAChar );
		}

		return ConvertedStr;
	}

	astring ConvertANSIColorCodes( const ustring &str )
	{
		return ConvertANSIColorCodes( str, achar() );
	}

	astring ConvertANSIColorCodes( const ustring &str,
								   const achar &initialANSIState )
	{
		astring ConvertedStr;

		if ( str.empty() )
			return ConvertedStr;

		achar CurAChar, CurANSIState(initialANSIState);
		auto s_begin = str.begin(), s_end = str.end();

		for ( auto CurChar = s_begin; CurChar != s_end; ++CurChar )
		{
			char CheckChar = !(CurChar->Glyph().empty()) ? (CurChar->Glyph()[0]) : ('\0');
			bool IsBold = false;

			// Check for ASCII control characters, etc.  Replace them with a ' ' (space) glyph.
			if ( CheckChar <= 32 )
				CheckChar = 32;

			// Check to see if we have a color sequence starting...if so...
			// ASCII escape = '\033'
			if ( CheckChar == '\033' )
			{
				auto SeqStart = CurChar;
				IsBold = false;				// We have to keep track because of differences between ANSI codes and AnsiGL handling

				if ( ++CurChar == s_end )
					break;

				CheckChar = !((*CurChar).Glyph().empty()) ? ((*CurChar).Glyph()[0]) : ('\0');

				// Make sure it's a proper ANSI escape sequence
				if ( CheckChar != '[' )
				{
					CurChar = SeqStart;
					continue;
				}

				std::string Code("");
				bool BreakLoop = false;

				while ( !BreakLoop && CurChar != s_end )
				{
					Code.clear();

					// Read digits until the next separator, storing them in Code
					for ( ++CurChar; CurChar != s_end; ++CurChar )
					{
						CheckChar = !((*CurChar).Glyph().empty()) ? ((*CurChar).Glyph()[0]) : ('\0');

						// Check for the code separator
						if ( CheckChar == ANSI_CODE_SEPARATOR )
						{
							break;
						}
						// Check for the end of the sequence
						else if ( CheckChar == ANSI_CODE_END || CheckChar == ']' ) // The closing bracket is optional, but should be handled
						{
							BreakLoop = true;

							// Grab the trailing ']' if it exists
							if ( CheckChar == 'm' )
							{
								if ( ++CurChar != s_end )
									CheckChar = !((*CurChar).Glyph().empty()) ? ((*CurChar).Glyph()[0]) : ('\0');

								// If that's not it, then leave it be
								if ( CheckChar != ']' )
									--CurChar;
							}

							break;
						}
						else
							Code.push_back( CheckChar );
					}

					int AnsiCode = std::atoi( Code.c_str() );

					//
					// Start our code checking with some sanity checking
					//
					if ( AnsiCode >= ANSI_Max )
					{
						SeqStart = CurChar;
						break;
					}

					//
					// Attributes
					//
					else if ( AnsiCode == ANSI_Default )
					{
						CurANSIState.Clear();
						IsBold = false;
					}
					else if ( AnsiCode == ANSI_BoldOn )
					{
						CurANSIState.RemoveAttribute( ANSI_BoldOff );
						CurANSIState.AddAttribute( ANSI_BoldOn );
						IsBold = true;
					}
					else if ( AnsiCode == ANSI_ItalicsOn )
					{
						CurANSIState.RemoveAttribute( ANSI_ItalicsOff );
						CurANSIState.AddAttribute( ANSI_ItalicsOn );
					}
					else if ( AnsiCode == ANSI_UnderlineOn )
					{
						CurANSIState.RemoveAttribute( ANSI_UnderlineOff );
						CurANSIState.AddAttribute( ANSI_UnderlineOn );
					}
					else if ( AnsiCode == ANSI_BlinkSlow )
					{
						CurANSIState.RemoveAttribute( ANSI_BlinkOff );
						CurANSIState.AddAttribute( ANSI_BlinkSlow );
					}
					else if ( AnsiCode == ANSI_BlinkFast )
					{
						CurANSIState.RemoveAttribute( ANSI_BlinkOff );
						CurANSIState.AddAttribute( ANSI_BlinkFast );
					}
					else if ( AnsiCode == ANSI_InverseOn )
					{
						CurANSIState.RemoveAttribute( ANSI_InverseOff );
						CurANSIState.AddAttribute( ANSI_InverseOn );
					}
					else if ( AnsiCode == ANSI_CrossedOut )
					{
						CurANSIState.RemoveAttribute( ANSI_CrossedOutOff );
						CurANSIState.AddAttribute( ANSI_CrossedOut );
					}
					else if ( AnsiCode == ANSI_BoldOff || AnsiCode == ANSI_NormalIntensity )
					{
						CurANSIState.RemoveAttribute( ANSI_BoldOn );
						CurANSIState.AddAttribute( ANSI_BoldOff );
						IsBold = false;
					}
					else if ( AnsiCode == ANSI_ItalicsOff )
					{
						CurANSIState.RemoveAttribute( ANSI_ItalicsOn );
						CurANSIState.AddAttribute( ANSI_ItalicsOff );
					}
					else if ( AnsiCode == ANSI_UnderlineOff )
					{
						CurANSIState.RemoveAttribute( ANSI_UnderlineOn );
						CurANSIState.AddAttribute( ANSI_UnderlineOff );
					}
					else if ( AnsiCode == ANSI_BlinkOff )
					{
						CurANSIState.RemoveAttribute( ANSI_BlinkSlow );
						CurANSIState.RemoveAttribute( ANSI_BlinkFast );
						CurANSIState.AddAttribute( ANSI_BlinkOff );
					}
					else if ( AnsiCode == ANSI_InverseOff )
					{
						CurANSIState.RemoveAttribute( ANSI_InverseOn );
						CurANSIState.AddAttribute( ANSI_InverseOff );
					}
					else if ( AnsiCode == ANSI_CrossedOutOff )
					{
						CurANSIState.RemoveAttribute( ANSI_CrossedOut );
						CurANSIState.AddAttribute( ANSI_CrossedOutOff );
					}

					//
					// Foreground colors
					//
					else if ( AnsiCode == ANSI_FG_Black )
					{
						if ( IsBold )
						{
							CurANSIState.RemoveAttribute( ANSI_BoldOn );		// The color covers it
							CurANSIState.Color.FG.Set( ANSISysColor_BoldBlack );
						}
						else
							CurANSIState.Color.FG.Set( ANSISysColor_Black );
					}
					else if ( AnsiCode == ANSI_FG_Red )
					{
						if ( IsBold )
						{
							CurANSIState.RemoveAttribute( ANSI_BoldOn );		// The color covers it
							CurANSIState.Color.FG.Set( ANSISysColor_BoldRed );
						}
						else
							CurANSIState.Color.FG.Set( ANSISysColor_Red );
					}
					else if ( AnsiCode == ANSI_FG_Green )
					{
						if ( IsBold )
						{
							CurANSIState.RemoveAttribute( ANSI_BoldOn );		// The color covers it
							CurANSIState.Color.FG.Set( ANSISysColor_BoldGreen );
						}
						else
							CurANSIState.Color.FG.Set( ANSISysColor_Green );
					}
					else if ( AnsiCode == ANSI_FG_Yellow )
					{
						if ( IsBold )
						{
							CurANSIState.RemoveAttribute( ANSI_BoldOn );		// The color covers it
							CurANSIState.Color.FG.Set( ANSISysColor_BoldYellow );
						}
						else
							CurANSIState.Color.FG.Set( ANSISysColor_Yellow );
					}
					else if ( AnsiCode == ANSI_FG_Blue )
					{
						if ( IsBold )
						{
							CurANSIState.RemoveAttribute( ANSI_BoldOn );		// The color covers it
							CurANSIState.Color.FG.Set( ANSISysColor_BoldBlue );
						}
						else
							CurANSIState.Color.FG.Set( ANSISysColor_Blue );
					}
					else if ( AnsiCode == ANSI_FG_Magenta )
					{
						if ( IsBold )
						{
							CurANSIState.RemoveAttribute( ANSI_BoldOn );		// The color covers it
							CurANSIState.Color.FG.Set( ANSISysColor_BoldMagenta );
						}
						else
							CurANSIState.Color.FG.Set( ANSISysColor_Magenta );
					}
					else if ( AnsiCode == ANSI_FG_Cyan )
					{
						if ( IsBold )
						{
							CurANSIState.RemoveAttribute( ANSI_BoldOn );		// The color covers it
							CurANSIState.Color.FG.Set( ANSISysColor_BoldCyan );
						}
						else
							CurANSIState.Color.FG.Set( ANSISysColor_Cyan );
					}
					else if ( AnsiCode == ANSI_FG_White )
					{
						if ( IsBold )
						{
							CurANSIState.RemoveAttribute( ANSI_BoldOn );		// The color covers it
							CurANSIState.Color.FG.Set( ANSISysColor_BoldWhite );
						}
						else
							CurANSIState.Color.FG.Set( ANSISysColor_White );
					}
					else if ( AnsiCode == ANSI_FG_Default )
					{
						CurANSIState.Color.FG.Set( ANSISysColor_Default );
					}

					//
					// Background colors
					//
					else if ( AnsiCode == ANSI_BG_Black )
						CurANSIState.Color.BG.Set( ANSISysColor_Black );
					else if ( AnsiCode == ANSI_BG_Red )
						CurANSIState.Color.BG.Set( ANSISysColor_Red );
					else if ( AnsiCode == ANSI_BG_Green )
						CurANSIState.Color.BG.Set( ANSISysColor_Green );
					else if ( AnsiCode == ANSI_BG_Yellow )
						CurANSIState.Color.BG.Set( ANSISysColor_Yellow );
					else if ( AnsiCode == ANSI_BG_Blue )
						CurANSIState.Color.BG.Set( ANSISysColor_Blue );
					else if ( AnsiCode == ANSI_BG_Magenta )
						CurANSIState.Color.BG.Set( ANSISysColor_Magenta );
					else if ( AnsiCode == ANSI_BG_Cyan )
						CurANSIState.Color.BG.Set( ANSISysColor_Cyan );
					else if ( AnsiCode == ANSI_BG_White )
						CurANSIState.Color.BG.Set( ANSISysColor_White );
					else if ( AnsiCode == ANSI_BG_Default )
						CurANSIState.Color.BG.Set( ANSISysColor_Default );
				}

				// Now that we've got that color code, just continue and press on like normal
				continue;
			}

			CurAChar = CurANSIState;
			CurAChar.Glyph( *CurChar );
			ConvertedStr.push_back( CurAChar );
			CurAChar.Clear();

			CurANSIState.Bell = false;		// Clear the bell each time so it isn't painted more than we actually want
		}

		return ConvertedStr;
	}


	//
	// achar
	//
	void achar::assign( const ustring &right,
						const achar &prevColor,
						const tColorCode *colorCodes )
	{
		Clear();

		if ( right.empty() )
			return;

		astring Temp = ConvertColorCodes( right, prevColor, colorCodes );

		if ( !Temp.empty() )
			(*this) = Temp[0];
	}

	void achar::assign( const std::string &right,
						const achar &prevColor,
						const tColorCode *colorCodes )
	{
		Clear();

		if ( right.empty() )
			return;

		astring Temp = ConvertColorCodes( ustring(right), prevColor, colorCodes );

		if ( !Temp.empty() )
			(*this) = Temp[0];
	}

	achar &achar::operator=( const ustring &right )
	{
		this->assign( right );
		return (*this);
	}

	achar &achar::operator=( const uchar &right )
	{
		// An achar can be passed as a uchar, so we need to check for self-assignment
		if ( this == &right )
			return (*this);

		Clear();

		Glyph( right.Glyph() );

		return (*this);
	}

	achar &achar::operator=( const std::string &right )
	{
		this->assign( right );
		return (*this);
	}

	bool achar::operator==( const achar &right ) const
	{
		if ( Glyph().compare( right.Glyph() ) )
			return false;

		if ( Color != right.Color )
			return false;

		if ( _Attributes != right.Attributes() )
			return false;

		/* -- Not sure if it should compare this or not...I'm thinking no
			if ( Bell != right.Bell )
			    return false;
		*/
		return true;
	}

	bool achar::operator!=( const achar &right ) const
	{
		return !((*this) == right);
	}

	bool achar::operator==( const ustring &right ) const
	{
		if ( Glyph().empty() && right.empty() )
			return true;
		else if ( right.empty() || right.length() > 1 )
			return false;

		return !( Glyph().compare((*right.begin()).Glyph()) );
	}

	bool achar::operator!=( const ustring &right ) const
	{
		return !((*this) == right);
	}

	bool achar::operator==( const uchar &right ) const
	{
		return (right == (*this));	// Invoke the uchar version
	}

	bool achar::operator!=( const uchar &right ) const
	{
		return !((*this) == right);
	}

	bool achar::operator==( const std::string &right ) const
	{
		return !( Glyph().compare(right) );
	}

	bool achar::operator!=( const std::string &right ) const
	{
		return !((*this) == right);
	}

	bool achar::operator==( const char &right ) const
	{
		return (Glyph().c_str()[0] == right);
	}

	bool achar::operator!=( const char &right ) const
	{
		return !((*this) == right);
	}

	void achar::CopyAttributes( const ANSICodeList &attrib )
	{
		_Attributes = attrib;
	}

	void achar::AddAttribute( ENUM_ANSICodes attrib )
	{
		// Don't allow foreground colors
		if ( attrib >= ANSI_FG_Black && attrib <= ANSI_FG_Default )
			return;

		// And don't allow background colors either
		if ( attrib >= ANSI_BG_Black && attrib <= ANSI_BG_Default )
			return;

		_Attributes.Add( attrib );
	}

	void achar::AddAttributes( const ANSICodeList &attrib )
	{
		// Make sure our banned attributes are initialized as expected
		initBannedAttributes();

		_Attributes.Add( attrib );
		_Attributes.Remove( *achar::_BannedAttributes );
	}

	void achar::RemoveAttribute( ENUM_ANSICodes attrib )
	{
		_Attributes.Remove( attrib );
	}

	void achar::RemoveAttributes( const ANSICodeList &attrib )
	{
		_Attributes.Remove( attrib );
	}

	bool achar::HasAnsi() const
	{
		return (!_Attributes.empty() || !Color.IsColorless());
	}

	void achar::Clear()
	{
		uchar::Clear();
		_Attributes.clear();
		Bell = false;
		Color.Clear();
	}

	std::string achar::RenderAnsi( ENUM_ColorDepth desiredDepth ) const
	{
		std::stringstream RenderedStr("");

		// If we have no color or attributes set, just render an ANSI default
		if ( Color.IsColorless() && _Attributes.empty() )
		{
			RenderedStr << ANSI_CODE_START << ANSI_Default << ANSI_CODE_END;
			return RenderedStr.str();
		}

		bool NeedANSICodeBegin = true;

		// Render our ANSI attributes
		if ( !_Attributes.empty() )
		{
			RenderedStr << ANSI_CODE_START << (int)ANSI_Default << ANSI_CODE_SEPARATOR;		// ANSI Code Begin and reset all ANSI styles
			NeedANSICodeBegin = false;					// Since we've officially started our sequence,

			RenderedStr << _Attributes.Render();
		}

		if ( !Color.IsColorless() )
		{
			if ( NeedANSICodeBegin )
			{
				RenderedStr << ANSI_CODE_START << (int)ANSI_Default << ANSI_CODE_SEPARATOR;
				NeedANSICodeBegin = false;
			}

			// Render our ANSI color
			RenderedStr << Color.Render( desiredDepth );
		}

		// At this point, we will probably need an ANSI terminator
		if ( !NeedANSICodeBegin )
			RenderedStr << ANSI_CODE_END;

		return RenderedStr.str();
	}

	std::string achar::Render( ENUM_ColorDepth desiredDepth, bool enableAnsi, bool enableBell ) const
	{
		std::stringstream RenderedStr("");

		if ( Bell && enableBell )
			RenderedStr << '\007';

		if ( enableAnsi )
			RenderedStr << RenderAnsi( desiredDepth );

		RenderedStr << Glyph();

		if ( enableAnsi )
			RenderedStr << ANSI_CODE_START << ANSI_Default << ANSI_CODE_END;

		return RenderedStr.str();
	}

	ANSICodeList::Ptr achar::_BannedAttributes;

	void achar::initBannedAttributes()
	{
		if ( _BannedAttributes )
			return;

		_BannedAttributes = ANSICodeList::Ptr( new ANSICodeList );

		// We don't allow colors since they will be stored separately
		_BannedAttributes->Add( ANSI_FG_Black );
		_BannedAttributes->Add( ANSI_FG_Red );
		_BannedAttributes->Add( ANSI_FG_Green );
		_BannedAttributes->Add( ANSI_FG_Yellow );
		_BannedAttributes->Add( ANSI_FG_Blue );
		_BannedAttributes->Add( ANSI_FG_Magenta );
		_BannedAttributes->Add( ANSI_FG_Cyan );
		_BannedAttributes->Add( ANSI_FG_White );
		_BannedAttributes->Add( ANSI_FG_Default );
		_BannedAttributes->Add( ANSI_BG_Black );
		_BannedAttributes->Add( ANSI_BG_Red );
		_BannedAttributes->Add( ANSI_BG_Green );
		_BannedAttributes->Add( ANSI_BG_Yellow );
		_BannedAttributes->Add( ANSI_BG_Blue );
		_BannedAttributes->Add( ANSI_BG_Magenta );
		_BannedAttributes->Add( ANSI_BG_Cyan );
		_BannedAttributes->Add( ANSI_BG_White );
		_BannedAttributes->Add( ANSI_BG_Default );
		_BannedAttributes->Add( ANSI_BG_256Color );
	}


	std::ostream &operator<<( std::ostream &left, const achar &right )
	{
		left << right.Render();
		return left;
	}
}


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --indent-switches --pad-oper


