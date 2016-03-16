#ifndef __ANSIGL_ACHAR_H__
#define __ANSIGL_ACHAR_H__

/*
 * achar.h
 *
 * Defines an ANSI-enabled "char".
 */


#include <string>

#include "color.h"
#include "uchar.h"


namespace AnsiGL
{
	//
	// Color Codes
	//
	// Color codes are easily hand-generated via a typeable escape sequence.
	// astring assignments (etc.) from std::strings are parsed for color
	//   codes.
	//
	// Color escape char: ^
	// Color list start char: {
	// Color list end char: }
	//
	// In a std::string, simply type the escape character, then the code.  Or, for
	//   multiple codes in a single sequence: type the escape character, the
	//   list start character, a sequence of color codes, and the list end
	//   character.
	//
	// Color Codes:
	//
	// 6-bit and 7-bit (no bold foreground for 6-bit):
	// FG Colors:				BG Colors:
	// k) Black		K) Bold Black		1) Black
	// r) Red		R) Bold Red		2) Red
	// y) Yellow	Y) Bold Yellow		3) Yellow
	// g) Green		G) Bold Green		4) Green
	// b) Blue		B) Bold Blue		5) Blue
	// m) Magenta	M) Bold Magenta		6) Magenta
	// c) Cyan		C) Bold Cyan		7) Cyan
	// w) White		W) Bold White		8) White
	// d) Default				0) Default
	//
	//
	// TODO:
	// TODO: Add a 'p' and 'P' code for "previous color" -- Just grabs the previous color and sets that as the current color
	// TODO: In addition to that, consider a code for saving and restoring colors...so I can prefix my strings with a "previous" color
	// TODO: Also add a code for CLS, and possibly codes for cursor movement as well.  Probably need to expand this into a system that can enable/disable codes easily.
	//
	//
	// 8-bit (includes all of the above):
	// #RGB[F|B]		-- Set by RGB values where R, G, and B get
	// 				   replaced with a value from 0-5 (ex: ^#050
	//				   would be very green), followed by a F or B
	//				   to denote foreground or background)
	// $[0-23][F|G]		-- Set a grayscale gradient level to either the
	// 				   foreground or background as well.
	// ^#{color name,[F|B]}	-- Set by the color's given name, also to either
	//				   the foreground or background.
	//
	// Attributes:
	// *) Bold
	// .) Bold Off
	// /) Italics
	// \) Italics Off
	// _) Underline On
	// -) Underline Off
	// %) CrossedOut On
	// =) CrossedOut Off
	// :) Blink On
	// ;) Blink Off
	// I) Invert Colors
	// i) Uninvert Colors
	//
	// Special Codes:
	// ^D) Restore all defaults
	// ^&) Bell character (beep, '\007')
	//
	// Examples of multi-character codes:
	//   ^{R1&}				- Bold Red on Black that beeps when
	//					  printed on-screen
	//   ^{/7g*}			- Italicized, Bold Green on Cyan
	//   ^{#500F#{dark blue,B}_}	- A very bright red foreground with
	//					  a dark blue background, underlined
	//
	// All functions in the astring class that take a std::string will
	//   convert color codes when creating a astring.
	//
	const char COLOR_ESCAPE_CHAR	= '^';
	const char COLOR_LIST_START_CHAR	= '{';
	const char COLOR_LIST_END_CHAR	= '}';

	const char COLOR_RGB		= '#';
	const char COLOR_GRAYSCALE		= '$';

	const char COLOR_BELL		= '&';

	const char COLOR_CLEAR_ALL		= 'D';

	const char COLOR_BLACK		= 'k';
	const char COLOR_RED		= 'r';
	const char COLOR_GREEN		= 'g';
	const char COLOR_YELLOW		= 'y';
	const char COLOR_BLUE		= 'b';
	const char COLOR_MAGENTA		= 'm';
	const char COLOR_CYAN		= 'c';
	const char COLOR_WHITE		= 'w';
	const char COLOR_DEFAULT		= 'd';

	const char COLOR_BOLD_BLACK		= 'K';
	const char COLOR_BOLD_RED		= 'R';
	const char COLOR_BOLD_GREEN		= 'G';
	const char COLOR_BOLD_YELLOW	= 'Y';
	const char COLOR_BOLD_BLUE		= 'B';
	const char COLOR_BOLD_MAGENTA	= 'M';
	const char COLOR_BOLD_CYAN		= 'C';
	const char COLOR_BOLD_WHITE		= 'W';

	const char COLOR_BG_BLACK		= '1';
	const char COLOR_BG_RED		= '2';
	const char COLOR_BG_GREEN		= '3';
	const char COLOR_BG_YELLOW		= '4';
	const char COLOR_BG_BLUE		= '5';
	const char COLOR_BG_MAGENTA		= '6';
	const char COLOR_BG_CYAN		= '7';
	const char COLOR_BG_WHITE		= '8';
	const char COLOR_BG_DEFAULT		= '0';

	const char COLOR_BOLD_ON		= '*';
	const char COLOR_BOLD_OFF		= '.';
	const char COLOR_ITALICS_ON		= '/';
	const char COLOR_ITALICS_OFF	= '\\';
	const char COLOR_UNDERLINE_ON	= '_';
	const char COLOR_UNDERLINE_OFF	= '-';
	const char COLOR_CROSSEDOUT_ON	= '%';
	const char COLOR_CROSSEDOUT_OFF	= '=';
	const char COLOR_BLINK_ON		= ':';
	const char COLOR_BLINK_OFF		= ';';

	const char COLOR_INVERT_ON		= 'I';
	const char COLOR_INVERT_OFF		= 'i';


	//
	// Forward declarations
	//
	class astring;
	class ustring;


	//
	// An ANSI-Enabled UTF-8 character
	//
	class achar : public uchar
	{
	protected:
		ANSICodeList		m_Attributes;		// Except color!

	public:
		ColorDef		Color;
		bool			Bell;

	public:
		achar();
		achar( const ColorDef &color );
		achar( const ColorDef &color, bool bell );
		achar( const ustring &glyphWithColor );		// This will convert the ustring for color, and take the first character from the resulting gstring as the achar
		achar( const uchar &glyph );
		achar( const std::string &glyphWithColor );	// This will convert the std::string for color, and take the first character from the resulting gstring as the achar
		achar( const char ch );

		achar &operator=( const ustring &right );	// Overloaded = for unicode std::strings, so we can convert on the fly via = as well
		achar &operator=( const uchar &right );		// Overloaded = for unicode characters too
		achar &operator=( const std::string &right );	// Overloaded = for std::strings, so we can convert on the fly via = as well
		achar &operator=( const char &right );

		inline achar &operator<<( const ustring &right )
		{
			return ((*this) = right);
		}

		inline achar &operator<<( const uchar &right )
		{
			return ((*this) = right);
		}

		inline achar &operator<<( const std::string &right )
		{
			return ((*this) = right);
		}

		inline achar &operator<<( const char &right )
		{
			return ((*this) = right);
		}

		bool operator==( const achar &right ) const;
		bool operator!=( const achar &right ) const;
		bool operator==( const ustring &right ) const;	// Compares the ustring as a single character with a color code
		bool operator!=( const ustring &right ) const;
		bool operator==( const uchar &right ) const;
		bool operator!=( const uchar &right ) const;
		bool operator==( const std::string &right ) const;	// Compares the std::string as a single character with a color code
		bool operator!=( const std::string &right ) const;
		bool operator==( const char &right ) const;
		bool operator!=( const char &right ) const;

		const ANSICodeList Attributes() const			// Give the compiler the option to inline
		{
			return m_Attributes;
		}

		bool HasAttribute( ENUM_ANSICodes attrib ) const
		{
			return m_Attributes.Has(attrib);
		}

		void CopyAttributes( const ANSICodeList &attrib );
		void AddAttribute( ENUM_ANSICodes attrib );
		void AddAttributes( const ANSICodeList &attrib );
		void RemoveAttribute( ENUM_ANSICodes attrib );
		void RemoveAttributes( const ANSICodeList &attrib );

		bool HasAnsi() const;				// Returns true if there is ANSI to render

		virtual void Clear();

		virtual std::string RenderAnsi( ENUM_ColorDepth desiredDepth = ColorDepth_Default ) const;	// Renders the complete ANSI code required for this character (and a bell character too, if needed)
		virtual std::string Render( ENUM_ColorDepth desiredDepth = ColorDepth_Default, bool enableAnsi = true, bool enableBell = true ) const;	// Renders the character as a properly formed ANSI-encoded std::string, complete with an ANSI terminator

		friend class Surface;	// For fast access

	protected:
		static ANSICodeList::Ptr m_BannedAttributes;

		void initBannedAttributes();
	};


	//
	// Global Functions
	//
	astring ConvertColorCodes( const ustring &str, const achar &initialANSIState = achar() );	// Converts a ustring with color codes into a astring, using initialState to pass the previous ANSI state (if desired)

	std::ostream &operator<<( std::ostream &left, const achar &right );
}

#endif // __ANSIGL_ACHAR_H__


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --pad-oper


