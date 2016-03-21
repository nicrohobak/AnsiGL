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
	// Global Functions
	//
	astring ConvertColorCodes( const ustring &str, const achar &initialANSIState )
	{
		astring ConvertedStr;

		if ( str.empty() )
			return ConvertedStr;

		achar CurAChar, CurANSIState(initialANSIState);
		ustring::const_iterator CurChar;

		for ( CurChar = str.begin(); CurChar != str.end(); ++CurChar )
		{
			char CheckChar = !(CurChar->Glyph().empty()) ? (CurChar->Glyph()[0]) : ('\0');

			// Check to see if we have a color sequence starting...if so...
			if ( CheckChar == COLOR_ESCAPE_CHAR )
			{
				bool BreakLoop = true;	// Default is true...the list start character sets false if we need more than just one pass
				bool ListSequence = false;

				while ( CurChar != str.end() )
				{
					++CurChar;		// Advance to the next character

					// Verify that we're okay to proceed
					if ( CurChar == str.end() )
						break;

					// Update our CheckChar
					CheckChar = !((*CurChar).Glyph().empty()) ? ((*CurChar).Glyph()[0]) : ('\0');

					switch ( CheckChar )
					{
						case COLOR_ESCAPE_CHAR:
							{
								if ( !ListSequence )
								{
									CurAChar = CurANSIState;
									CurAChar.Glyph( COLOR_ESCAPE_CHAR );
									ConvertedStr.push_back( CurAChar );
									CurAChar.Clear();
								}
								break;
							}
						case COLOR_LIST_START_CHAR:
							ListSequence = true;
							BreakLoop = false;
							break;
						case COLOR_LIST_END_CHAR:
							if ( ListSequence )
								BreakLoop = true;
							break;

						case COLOR_BELL:
							CurANSIState.Bell = true;
							break;

						case COLOR_RGB:
						case COLOR_GRAYSCALE:
							{
								bool IsRGB = CheckChar == COLOR_RGB;

								// Check for foreground/background
								bool Background = false;

								std::string Digits("");

								// Read digits until the first non-digit, storing them in Digits
								for ( ++CurChar; CurChar != str.end(); ++CurChar )
								{
									CheckChar = !((*CurChar).Glyph().empty()) ? ((*CurChar).Glyph()[0]) : ('\0');

									if ( !isdigit( CheckChar ) )
										break;

									Digits.push_back( CheckChar );
								}

								if ( CurChar == str.end() )
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

						case COLOR_CLEAR_ALL:
							{
								ANSIColorPalette::Ptr FGPalette = CurANSIState.Color.FG.Palette();
								ANSIColorPalette::Ptr BGPalette = CurANSIState.Color.BG.Palette();
								CurANSIState.Clear();
								CurANSIState.Color.FG.Palette( FGPalette );
								CurANSIState.Color.BG.Palette( BGPalette );
								break;
							}

						case COLOR_BLACK:
							CurANSIState.Color.FG.Set( ANSISysColor_Black );
							break;
						case COLOR_RED:
							CurANSIState.Color.FG.Set( ANSISysColor_Red );
							break;
						case COLOR_GREEN:
							CurANSIState.Color.FG.Set( ANSISysColor_Green );
							break;
						case COLOR_YELLOW:
							CurANSIState.Color.FG.Set( ANSISysColor_Yellow );
							break;
						case COLOR_BLUE:
							CurANSIState.Color.FG.Set( ANSISysColor_Blue );
							break;
						case COLOR_MAGENTA:
							CurANSIState.Color.FG.Set( ANSISysColor_Magenta );
							break;
						case COLOR_CYAN:
							CurANSIState.Color.FG.Set( ANSISysColor_Cyan );
							break;
						case COLOR_WHITE:
							CurANSIState.Color.FG.Set( ANSISysColor_White );
							break;
						case COLOR_DEFAULT:
							CurANSIState.Color.FG.Set( ANSISysColor_Default );
							break;

						case COLOR_BOLD_BLACK:
							CurANSIState.Color.FG.Set( ANSISysColor_BoldBlack );
							break;
						case COLOR_BOLD_RED:
							CurANSIState.Color.FG.Set( ANSISysColor_BoldRed );
							break;
						case COLOR_BOLD_GREEN:
							CurANSIState.Color.FG.Set( ANSISysColor_BoldGreen );
							break;
						case COLOR_BOLD_YELLOW:
							CurANSIState.Color.FG.Set( ANSISysColor_BoldYellow );
							break;
						case COLOR_BOLD_BLUE:
							CurANSIState.Color.FG.Set( ANSISysColor_BoldBlue );
							break;
						case COLOR_BOLD_MAGENTA:
							CurANSIState.Color.FG.Set( ANSISysColor_BoldMagenta );
							break;
						case COLOR_BOLD_CYAN:
							CurANSIState.Color.FG.Set( ANSISysColor_BoldCyan );
							break;
						case COLOR_BOLD_WHITE:
							CurANSIState.Color.FG.Set( ANSISysColor_BoldWhite );
							break;

						case COLOR_BG_BLACK:
							CurANSIState.Color.BG.Set( ANSISysColor_Black );
							break;
						case COLOR_BG_RED:
							CurANSIState.Color.BG.Set( ANSISysColor_Red );
							break;
						case COLOR_BG_GREEN:
							CurANSIState.Color.BG.Set( ANSISysColor_Green );
							break;
						case COLOR_BG_YELLOW:
							CurANSIState.Color.BG.Set( ANSISysColor_Yellow );
							break;
						case COLOR_BG_BLUE:
							CurANSIState.Color.BG.Set( ANSISysColor_Blue );
							break;
						case COLOR_BG_MAGENTA:
							CurANSIState.Color.BG.Set( ANSISysColor_Magenta );
							break;
						case COLOR_BG_CYAN:
							CurANSIState.Color.BG.Set( ANSISysColor_Cyan );
							break;
						case COLOR_BG_WHITE:
							CurANSIState.Color.BG.Set( ANSISysColor_White );
							break;
						case COLOR_BG_DEFAULT:
							CurANSIState.Color.BG.Set( ANSISysColor_BGDefault );
							break;

						case COLOR_BOLD_ON:
							CurANSIState.RemoveAttribute( ANSI_BoldOff );
							CurANSIState.AddAttribute( ANSI_BoldOn );
							break;
						case COLOR_BOLD_OFF:
							CurANSIState.RemoveAttribute( ANSI_BoldOn );
							CurANSIState.AddAttribute( ANSI_BoldOff );
							break;
						case COLOR_ITALICS_ON:
							CurANSIState.RemoveAttribute( ANSI_ItalicsOff );
							CurANSIState.AddAttribute( ANSI_ItalicsOn );
							break;
						case COLOR_ITALICS_OFF:
							CurANSIState.RemoveAttribute( ANSI_ItalicsOn );
							CurANSIState.AddAttribute( ANSI_ItalicsOff );
							break;
						case COLOR_UNDERLINE_ON:
							CurANSIState.RemoveAttribute( ANSI_UnderlineOff );
							CurANSIState.AddAttribute( ANSI_UnderlineOn );
							break;
						case COLOR_UNDERLINE_OFF:
							CurANSIState.RemoveAttribute( ANSI_UnderlineOn );
							CurANSIState.AddAttribute( ANSI_UnderlineOff );
							break;
						case COLOR_CROSSEDOUT_ON:
							CurANSIState.RemoveAttribute( ANSI_CrossedOutOff );
							CurANSIState.AddAttribute( ANSI_CrossedOut );
							break;
						case COLOR_CROSSEDOUT_OFF:
							CurANSIState.RemoveAttribute( ANSI_CrossedOut );
							CurANSIState.AddAttribute( ANSI_CrossedOutOff );
							break;
						case COLOR_BLINK_ON:
							CurANSIState.RemoveAttribute( ANSI_BlinkOff );
							CurANSIState.AddAttribute( ANSI_BlinkSlow );	// Usually the only one implemented anyway...
							break;
						case COLOR_BLINK_OFF:
							CurANSIState.RemoveAttribute( ANSI_BlinkSlow );
							CurANSIState.AddAttribute( ANSI_BlinkOff );
							break;
						case COLOR_INVERT_ON:
							CurANSIState.RemoveAttribute( ANSI_InverseOff );
							CurANSIState.AddAttribute( ANSI_InverseOn );
							break;
						case COLOR_INVERT_OFF:
							CurANSIState.RemoveAttribute( ANSI_InverseOn );
							CurANSIState.AddAttribute( ANSI_InverseOff );
							break;

						default:
							break;
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


	//
	// achar
	//
	achar &achar::operator=( const ustring &right )
	{
		Clear();

		if ( right.empty() )
			return (*this);

		astring Temp = ConvertColorCodes( right );

		if ( !Temp.empty() )
			(*this) = Temp[0];

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
		Clear();

		if ( right.empty() )
			return (*this);

		astring Temp = ConvertColorCodes( ustring(right) );

		if ( !Temp.empty() )
			(*this) = Temp[0];

		return (*this);
	}

	achar &achar::operator=( const char &right )
	{
		Clear();

		if ( right >= 0 && right < 128 )
			Glyph( right );

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


