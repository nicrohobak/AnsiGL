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
	// Default Color Codes
	//
	// Color codes are easily hand-generated via a typeable escape sequence.
	// astring assignments (etc.) from std::strings are parsed for color
	//   codes.
	//
	// Color escape char:		^
	// Color list start char:	{
	// Color list end char:		}
	//
	// In a std::string, simply type the escape character, then the code.  Or, for
	//   multiple codes in a single sequence: type the escape character, the
	//   list start character, a sequence of color codes, and the list end
	//   character.
	//
	// Color Codes:
	//
	// 6-bit and 7-bit (no bold foreground for 6-bit):
	// FG Colors:					BG Colors:
	// k) Black		K) Bold Black	1) Black
	// r) Red		R) Bold Red		2) Red
	// y) Yellow	Y) Bold Yellow	3) Yellow
	// g) Green		G) Bold Green	4) Green
	// b) Blue		B) Bold Blue	5) Blue
	// m) Magenta	M) Bold Magenta	6) Magenta
	// c) Cyan		C) Bold Cyan	7) Cyan
	// w) White		W) Bold White	8) White
	// d) Default					0) Default
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
	//   ^{R1&}						- Bold Red on Black that beeps when
	//								  printed on-screen
	//   ^{/7g*}					- Italicized, Bold Green on Cyan
	//   ^{#500F#{dark blue,B}_}	- A very bright red foreground with
	//								  a dark blue background, underlined
	//
	// All functions in the astring class that take a std::string will
	//   convert color codes when creating a astring.
	//

	enum tColorCodeIndex
	{
		ColorCode_FIRST,

		ColorCode_Escape		= ColorCode_FIRST,
		ColorCode_List_Start,
		ColorCode_List_End,

		ColorCode_RGB,
		ColorCode_Grayscale,

		ColorCode_Bell,

		ColorCode_Clear_All,

		ColorCode_Black,
		ColorCode_Red,
		ColorCode_Green,
		ColorCode_Yellow,
		ColorCode_Blue,
		ColorCode_Magenta,
		ColorCode_Cyan,
		ColorCode_White,
		ColorCode_Default,

		ColorCode_Bold_Black,
		ColorCode_Bold_Red,
		ColorCode_Bold_Green,
		ColorCode_Bold_Yellow,
		ColorCode_Bold_Blue,
		ColorCode_Bold_Magenta,
		ColorCode_Bold_Cyan,
		ColorCode_Bold_White,

		ColorCode_BG_Black,
		ColorCode_BG_Red,
		ColorCode_BG_Green,
		ColorCode_BG_Yellow,
		ColorCode_BG_Blue,
		ColorCode_BG_Magenta,
		ColorCode_BG_Cyan,
		ColorCode_BG_White,
		ColorCode_BG_Default,

		ColorCode_Bold_On,
		ColorCode_Bold_Off,
		ColorCode_Italics_On,
		ColorCode_Italics_Off,
		ColorCode_Underline_On,
		ColorCode_Underline_Off,
		ColorCode_CrossedOut_On,
		ColorCode_CrossedOut_Off,
		ColorCode_Blink_On,
		ColorCode_Blink_Off,
		ColorCode_Invert_On,
		ColorCode_Invert_Off,

		ColorCode_MAX,

		ColorCode_DEFAULT = ColorCode_FIRST,
	};

	typedef char			tColorCode;
	extern const tColorCode	*ActiveColorCodes;
	extern const tColorCode	DefaultColorCodes[ ColorCode_MAX ];


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
	public:
		ColorDef			Color;
		bool				Bell;

	public:
		achar():
			Bell( false )
		{
		}

		achar( const ColorDef &color ):
			Color( color ),
			Bell( false )
		{
		}

		achar( const ColorDef &color, bool bell ):
			Color( color ),
			Bell( bell )
		{
		}

		// This will convert the ustring for color, and take the first character from the resulting gstring as the achar
		achar( const ustring &glyphWithColor, const achar &prevColor = achar(), const tColorCode *colorCodes = DefaultColorCodes ):
			Bell( false )
		{
			this->assign( glyphWithColor, prevColor, colorCodes );
		}

		// This will convert the std::string for color, and take the first character from the resulting gstring as the achar
		achar( const std::string &glyphWithColor, const achar &prevColor = achar(), const tColorCode *colorCodes = DefaultColorCodes ):
			Bell( false )
		{
			this->assign( glyphWithColor, prevColor, colorCodes );
		}

		achar( const uchar &glyph ):
			Bell( false )
		{
			(*this) = glyph;
		}

		achar( const char ch ):
			Bell( false )
		{
			(*this) = uchar(ch);
		}

		void assign( const ustring &right, const achar &prevColor = achar(),  const tColorCode *colorCodes = DefaultColorCodes );
		void assign( const std::string &right, const achar &prevColor = achar(),  const tColorCode *colorCodes = DefaultColorCodes );

		achar &operator=( const ustring &right );			// Overloaded = for unicode std::strings, so we can convert on the fly via = as well
		achar &operator=( const uchar &right );				// Overloaded = for unicode characters too
		achar &operator=( const std::string &right );		// Overloaded = for std::strings, so we can convert on the fly via = as well

		achar &operator<<( const ustring &right )
		{
			return ((*this) = right);
		}

		achar &operator<<( const uchar &right )
		{
			return ((*this) = right);
		}

		achar &operator<<( const std::string &right )
		{
			return ((*this) = right);
		}

		achar &operator<<( const char *right )
		{
			return ((*this) = right);
		}

		achar &operator<<( const char right )
		{
			return ((*this) = achar(right));
		}

		bool operator==( const achar &right ) const;
		bool operator!=( const achar &right ) const;
		bool operator==( const ustring &right ) const;		// Compares the ustring as a single character with a color code
		bool operator!=( const ustring &right ) const;
		bool operator==( const uchar &right ) const;
		bool operator!=( const uchar &right ) const;
		bool operator==( const std::string &right ) const;	// Compares the std::string as a single character with a color code
		bool operator!=( const std::string &right ) const;
		bool operator==( const char &right ) const;
		bool operator!=( const char &right ) const;

		const ANSICodeList Attributes() const
		{
			return _Attributes;
		}

		bool HasAttribute( ENUM_ANSICodes attrib ) const
		{
			return _Attributes.Has(attrib);
		}

		void CopyAttributes( const ANSICodeList &attrib );
		void AddAttribute( ENUM_ANSICodes attrib );
		void AddAttributes( const ANSICodeList &attrib );
		void RemoveAttribute( ENUM_ANSICodes attrib );
		void RemoveAttributes( const ANSICodeList &attrib );

		bool HasAnsi() const;							// Returns true if there is ANSI to render

		virtual void Clear();

		// Renders the complete ANSI code required for this character (and a bell character too, if needed)
		virtual std::string RenderAnsi( ENUM_ColorDepth desiredDepth = ColorDepth_Default ) const;

		// Renders the character as a properly formed ANSI-encoded std::string, complete with an ANSI terminator
		virtual std::string Render( ENUM_ColorDepth desiredDepth = ColorDepth_Default,
									bool enableAnsi = true,
									bool enableBell = true ) const;

		friend class Surface;							// For fast access

	protected:
		static ANSICodeList::Ptr	_BannedAttributes;

	protected:
		ANSICodeList				_Attributes;		// Except color!

		void initBannedAttributes();
	};


	//
	// Global Functions
	//
	// Converts a ustring with color codes into a astring, using initialState to pass the previous ANSI state (if desired)
	astring ConvertColorCodes( const ustring &str,
							   const tColorCode *colorCodes = NULL );
	astring ConvertColorCodes( const ustring &str,
							   const achar &initialANSIState,
							   const tColorCode *colorCodes = NULL );

	// Similar to ConvertColorCodes(), but handles the standard ANSI escape codes instead
	astring ConvertANSIColorCodes( const ustring &str );
	astring ConvertANSIColorCodes( const ustring &str,
								   const achar &initialANSIState );

	std::ostream &operator<<( std::ostream &left, const achar &right );
}

#endif // __ANSIGL_ACHAR_H__


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --indent-switches --pad-oper


