/*
 * ansi.cpp
 *
 * ANSI definitions.
 */


#include <sstream>

#include "AnsiGL/ansi.h"
#include "AnsiGL/color.h"


namespace AnsiGL
{
	//
	// ANSI Constants
	//
	const char *ANSI_CODE_START		= "\033[";
	const char ANSI_CODE_END		= 'm';
	const char ANSI_CODE_SEPARATOR	= ';';
	const int  ANSI_BG_OFFSET		= 10;


	std::string ANSICodeList::Render( ENUM_ColorDepth desiredDepth, bool background ) const
	{
		std::stringstream RenderedStr("");
		bool FirstAttribute = true;

		for ( int i = ANSI_First; i < ANSI_Max; ++i )
		{
			if ( !Has((ENUM_ANSICodes)i) )
				continue;

			switch ( i )
			{
			// 8-bit not supported here...not enough info at this scope
			case ANSI_FG_256Color:
			case ANSI_BG_256Color:
				break;

			case ANSI_BoldOn:
			case ANSI_BoldOff:
			case ANSI_FaintOn:
			case ANSI_NormalIntensity:
				{
					// These particular codes don't really do anything in this one mode
					if ( !background && desiredDepth != ColorDepth_6Bit )
					{
						if ( !FirstAttribute )
							RenderedStr << ANSI_CODE_SEPARATOR;
						else
							FirstAttribute = false;

						RenderedStr << i;
					}

					break;
				}

			default:
				{
					if ( !FirstAttribute )
						RenderedStr << ANSI_CODE_SEPARATOR;
					else
						FirstAttribute = false;

					if ( background && (i >= ANSI_FG_Start && i <= ANSI_FG_End) )
						RenderedStr << (i + ANSI_BG_OFFSET);
					else if ( !background && (i >= ANSI_BG_Start && i <= ANSI_BG_End) )
						RenderedStr << (i - ANSI_BG_OFFSET);
					else
						RenderedStr << i;

					break;
				}
			}
		}

		return RenderedStr.str();
	}


	ANSIColorDef::ANSIColorDef():
		m_Index(-1),
		m_SystemColor(ANSISysColor_Default),
		m_R(0),
		m_G(0),
		m_B(0),
		m_Grayscale(0),
		Name(DEFAULT_ANSICOLOR_NAME)
	{
	}

	ANSIColorDef::ANSIColorDef( const ANSIColorDef &color ):
		m_Index(-1),
		m_SystemColor(ANSISysColor_Default),
		m_R(0),
		m_G(0),
		m_B(0),
		m_Grayscale(0),
		Name(DEFAULT_ANSICOLOR_NAME)
	{
		m_Index = color.m_Index;
		m_SystemColor = color.m_SystemColor;
		m_R = color.m_R;
		m_G = color.m_G;
		m_B = color.m_B;
		m_Grayscale = color.m_Grayscale;
		Name = color.Name;

		m_ANSI.Add( color.m_ANSI );
	}

	ANSIColorDef::ANSIColorDef( const std::string &name, ENUM_ANSISystemColors color ):
		m_Index(-1),
		m_SystemColor(ANSISysColor_Default),
		m_R(0),
		m_G(0),
		m_B(0),
		m_Grayscale(0),
		Name(name)
	{
		SystemColor( color );
	}

	ANSIColorDef::ANSIColorDef( const std::string &name, unsigned char r, unsigned char g, unsigned char b ):
		m_Index(-1),
		m_SystemColor(ANSISysColor_Default),
		m_R(r),
		m_G(g),
		m_B(b),
		m_Grayscale(0),
		Name(name)
	{
		calculateIndexFromRGB();
	}

	ANSIColorDef::ANSIColorDef( const std::string &name, unsigned char grayscale ):
		m_Index(-1),
		m_SystemColor(ANSISysColor_Default),
		m_R(0),
		m_G(0),
		m_B(0),
		m_Grayscale(grayscale),
		Name(name)
	{
		calculateIndexFromGrayscale();
	}

	ANSIColorDef::ANSIColorDef( ENUM_ANSISystemColors color ):
		m_Index(-1),
		m_SystemColor(ANSISysColor_Default),
		m_R(0),
		m_G(0),
		m_B(0),
		m_Grayscale(0),
		Name(DEFAULT_ANSICOLOR_NAME)
	{
		SystemColor( color );
	}

	ANSIColorDef::ANSIColorDef( unsigned char r, unsigned char g, unsigned char b ):
		m_Index(-1),
		m_SystemColor(ANSISysColor_Default),
		m_R(r),
		m_G(g),
		m_B(b),
		m_Grayscale(0),
		Name(DEFAULT_ANSICOLOR_NAME)
	{
		calculateIndexFromRGB();
	}

	ANSIColorDef::ANSIColorDef( unsigned char grayscale ):
		m_Index(-1),
		m_SystemColor(ANSISysColor_Default),
		m_R(0),
		m_G(0),
		m_B(0),
		m_Grayscale(grayscale),
		Name(DEFAULT_ANSICOLOR_NAME)
	{
		calculateIndexFromGrayscale();
	}

	bool ANSIColorDef::operator==( const ANSIColorDef &right ) const
	{
		if ( this == &right )
			return true;

		if ( m_Index == right.m_Index && m_ANSI == right.m_ANSI )
			return true;

		return false;
	}

	bool ANSIColorDef::operator!=( const ANSIColorDef &right ) const
	{
		return !((*this) == right);
	}

	ANSICodeList ANSIColorDef::ANSI() const
	{
		return m_ANSI;
	}

	unsigned char ANSIColorDef::Index() const
	{
		return m_Index;
	}

	ENUM_ANSISystemColors ANSIColorDef::SystemColor() const
	{
		return m_SystemColor;
	}

	void ANSIColorDef::SystemColor( ENUM_ANSISystemColors color )
	{
		bool Bold = false;

		m_ANSI.clear();
		m_SystemColor = color;

		switch ( color )
		{
		case ANSISysColor_BoldBlack:
			Bold = true;				// Make note that we're bold so we can set our index, then...
			m_ANSI.Add( ANSI_BoldOn );	// Intentional fall-through to set the color
		case ANSISysColor_Black:
			m_ANSI.Add( ANSI_FG_Black );
			m_Index = 0;
			break;

		case ANSISysColor_BoldRed:
			Bold = true;				// Make note that we're bold so we can set our index, then...
			m_ANSI.Add( ANSI_BoldOn );	// Intentional fall-through to set the color
		case ANSISysColor_Red:
			m_ANSI.Add( ANSI_FG_Red );
			m_Index = 1;
			break;

		case ANSISysColor_BoldGreen:
			Bold = true;				// Make note that we're bold so we can set our index, then...
			m_ANSI.Add( ANSI_BoldOn );	// Intentional fall-through to set the color
		case ANSISysColor_Green:
			m_ANSI.Add( ANSI_FG_Green );
			m_Index = 2;
			break;

		case ANSISysColor_BoldYellow:
			Bold = true;				// Make note that we're bold so we can set our index, then...
			m_ANSI.Add( ANSI_BoldOn );	// Intentional fall-through to set the color
		case ANSISysColor_Yellow:
			m_ANSI.Add( ANSI_FG_Yellow );
			m_Index = 3;
			break;

		case ANSISysColor_BoldBlue:
			Bold = true;				// Make note that we're bold so we can set our index, then...
			m_ANSI.Add( ANSI_BoldOn );	// Intentional fall-through to set the color
		case ANSISysColor_Blue:
			m_ANSI.Add( ANSI_FG_Blue );
			m_Index = 4;
			break;

		case ANSISysColor_BoldMagenta:
			Bold = true;				// Make note that we're bold so we can set our index, then...
			m_ANSI.Add( ANSI_BoldOn );	// Intentional fall-through to set the color
		case ANSISysColor_Magenta:
			m_ANSI.Add( ANSI_FG_Magenta );
			m_Index = 5;
			break;

		case ANSISysColor_BoldCyan:
			Bold = true;				// Make note that we're bold so we can set our index, then...
			m_ANSI.Add( ANSI_BoldOn );	// Intentional fall-through to set the color
		case ANSISysColor_Cyan:
			m_ANSI.Add( ANSI_FG_Cyan );
			m_Index = 6;
			break;

		case ANSISysColor_BoldWhite:
			Bold = true;				// Make note that we're bold so we can set our index, then...
			m_ANSI.Add( ANSI_BoldOn );	// Intentional fall-through to set the color
		case ANSISysColor_White:
			m_ANSI.Add( ANSI_FG_White );
			m_Index = 7;
			break;

		default:
			break;
		}

		// Bold characters are simply 8 above their non-bold counterparts in 256 color mode
		if ( Bold )
			m_Index += 8;
	}

	void ANSIColorDef::GetRGB( unsigned char *r, unsigned char *g, unsigned char *b ) const
	{
		if ( r )
			*r = m_R;

		if ( g )
			*g = m_G;

		if ( b )
			*b = m_B;
	}

	void ANSIColorDef::SetRGB( unsigned char r, unsigned char g, unsigned char b )
	{
		m_R = r;
		m_G = g;
		m_B = b;

		calculateIndexFromRGB();
	}

	unsigned char ANSIColorDef::Grayscale() const
	{
		return m_Grayscale;
	}

	void ANSIColorDef::Grayscale( unsigned char gradient )
	{
		m_Grayscale = gradient;

		calculateIndexFromGrayscale();
	}

	bool ANSIColorDef::IsSysColor() const
	{
		return (m_Index != -1 && m_Index <= 15);
	}

	bool ANSIColorDef::IsRGB() const
	{
		return (m_Index != -1 && m_Index >= 16 && m_Index <= 231);
	}

	bool ANSIColorDef::IsGrayscale() const
	{
		return (m_Index != -1 && m_Index >= 232);
	}

	void ANSIColorDef::Clear()
	{
		m_ANSI.clear();
		m_Index = -1;
		m_SystemColor = ANSISysColor_Default;
		m_R = 0;
		m_G = 0;
		m_B = 0;
		m_Grayscale = 0;
	}

	std::string ANSIColorDef::Render( ENUM_ColorDepth desiredDepth, bool background ) const
	{
		std::stringstream RenderedStr("");

		if ( m_Index == -1 )
			return RenderedStr.str();

		if ( desiredDepth == ColorDepth_8Bit && m_Index >= 16 )
		{
			if ( background )
				RenderedStr << ANSI_BG_256Color << ANSI_CODE_SEPARATOR << "5" << ANSI_CODE_SEPARATOR << (int)Index();
			else
				RenderedStr << ANSI_FG_256Color << ANSI_CODE_SEPARATOR << "5" << ANSI_CODE_SEPARATOR << (int)Index();

			return RenderedStr.str();
		}

		RenderedStr << m_ANSI.Render( desiredDepth, background );
		return RenderedStr.str();
	}

	void ANSIColorDef::calculateIndexFromRGB()
	{
		// Make sure the values are within bounds
		if ( m_R > 5 )
			m_R = 5;

		if ( m_G > 5 )
			m_G = 5;

		if ( m_B > 5 )
			m_B = 5;

		// TODO: Improve this...this algorithm probably isn't ideal...
		// Set the closest system color approximation we can
		if ( m_R > 3 && m_G > 3 && m_B > 3 )
			m_SystemColor = ANSISysColor_BoldWhite;
		else if ( m_G > 3 && m_B > 3 && m_R <= 3 )
			m_SystemColor = ANSISysColor_BoldCyan;
		else if ( m_R > 3 && m_B > 3 && m_G <= 3 )
			m_SystemColor = ANSISysColor_BoldMagenta;
		else if ( m_R > 3 && m_G > 3 && m_B <= 3 )
			m_SystemColor = ANSISysColor_BoldYellow;
		else if ( m_B > 3 && m_R <= 3 && m_G <= 3 )
			m_SystemColor = ANSISysColor_BoldBlue;
		else if ( m_G > 3 && m_R <= 3 && m_B <= 3 )
			m_SystemColor = ANSISysColor_BoldGreen;
		else if ( m_R > 3 && m_G <= 3 && m_B <= 3 )
			m_SystemColor = ANSISysColor_BoldRed;
		else if ( m_R >= 3 && m_G >= 3 && m_B >= 3 )
			m_SystemColor = ANSISysColor_BoldBlack;
		else if ( m_R > 0 && m_G > 0 && m_B > 0 )
			m_SystemColor = ANSISysColor_White;
		else if ( m_G > 0 && m_B > 0 && m_R == 0 )
			m_SystemColor = ANSISysColor_Cyan;
		else if ( m_R > 0 && m_B > 0 && m_G == 0 )
			m_SystemColor = ANSISysColor_Magenta;
		else if ( m_R > 0 && m_G > 0 && m_B == 0 )
			m_SystemColor = ANSISysColor_Yellow;
		else if ( m_B > 0 && m_R == 0 && m_G == 0 )
			m_SystemColor = ANSISysColor_Blue;
		else if ( m_G > 0 && m_R == 0 && m_B == 0 )
			m_SystemColor = ANSISysColor_Green;
		else if ( m_R > 0 && m_G == 0 && m_B == 0 )
			m_SystemColor = ANSISysColor_Red;
		else
			m_SystemColor = ANSISysColor_Black;

		SystemColor( m_SystemColor );				// This will update m_ANSI

		// A range of 0-5 gives us a total size of 6, hence the powers of six in the calculation
		// The +16 comes from an offset in the 256 color palette...the first 16 colors are the standard ANSI colors
		m_Index = (m_R * 36) + (m_G * 6) + (m_B) + 16;		// And this will properly set the index afterward

		// Set m_Grayscale to 0 now, since we're not using it
		m_Grayscale = 0;
	}

	void ANSIColorDef::calculateIndexFromGrayscale()
	{
		// Make sure the values are within bounds
		if ( m_Grayscale > 23 )
			m_Grayscale = 23;

		// TODO: Improve this too...  (This one probably needs it a little worse than the RGB values too...)
		// Set the closest system color approximation we can
		m_SystemColor = ANSISysColor_BoldWhite;	// This is also a bad algorithm for this...

		SystemColor( m_SystemColor );		// This will update m_ANSI

		// The +232 comes from an offset in the 256 color palette to the grayscale area
		m_Index = m_Grayscale + 232;		// And this will properly set the index afterward

		// Set each RGB value to 0 since we're not using them
		m_R = m_G = m_B = 0;
	}


	ANSIColorPalette::ANSIColorPalette()
	{
	}

	ANSIColorPalette::Ptr ANSIColorPalette::Master;

	void ANSIColorPalette::InitMasterPalette()
	{
		// If we already have something initalized, then don't stomp over it...
		if ( Master )
			return;

		Master = Ptr( new ANSIColorPalette );

		Master->push_back( ANSIColorDef::Ptr( new ANSIColorDef("Black", ANSISysColor_Black) ) );
		Master->push_back( ANSIColorDef::Ptr( new ANSIColorDef("Red", ANSISysColor_Red) ) );
		Master->push_back( ANSIColorDef::Ptr( new ANSIColorDef("Green", ANSISysColor_Green) ) );
		Master->push_back( ANSIColorDef::Ptr( new ANSIColorDef("Yellow", ANSISysColor_Yellow) ) );
		Master->push_back( ANSIColorDef::Ptr( new ANSIColorDef("Blue", ANSISysColor_Blue) ) );
		Master->push_back( ANSIColorDef::Ptr( new ANSIColorDef("Magenta", ANSISysColor_Magenta) ) );
		Master->push_back( ANSIColorDef::Ptr( new ANSIColorDef("Cyan", ANSISysColor_Cyan) ) );
		Master->push_back( ANSIColorDef::Ptr( new ANSIColorDef("White", ANSISysColor_White) ) );
		Master->push_back( ANSIColorDef::Ptr( new ANSIColorDef("Bold Black", ANSISysColor_BoldBlack) ) );
		Master->push_back( ANSIColorDef::Ptr( new ANSIColorDef("Bold Red", ANSISysColor_BoldRed) ) );
		Master->push_back( ANSIColorDef::Ptr( new ANSIColorDef("Bold Green", ANSISysColor_BoldGreen) ) );
		Master->push_back( ANSIColorDef::Ptr( new ANSIColorDef("Bold Yellow", ANSISysColor_BoldYellow) ) );
		Master->push_back( ANSIColorDef::Ptr( new ANSIColorDef("Bold Blue", ANSISysColor_BoldBlue) ) );
		Master->push_back( ANSIColorDef::Ptr( new ANSIColorDef("Bold Magenta", ANSISysColor_BoldMagenta) ) );
		Master->push_back( ANSIColorDef::Ptr( new ANSIColorDef("Bold Cyan", ANSISysColor_BoldCyan) ) );
		Master->push_back( ANSIColorDef::Ptr( new ANSIColorDef("Bold White", ANSISysColor_BoldWhite) ) );

// Temporary macro
		// This will make our color name lists easier to type/manage
#define AnsiGL_ISRGB(red,green,blue)	(r == (red) && g == (green) && b == (blue))
// Temporary macro

		// 0-5 for a span of 6 values for each R, G, and B.
		for ( unsigned char r = 0; r < 6; ++r )
		{
			for ( unsigned char g = 0; g < 6; ++g )
			{
				for ( unsigned char b = 0; b < 6; ++b )
				{
					ANSIColorDef::Ptr CurColor = ANSIColorDef::Ptr( new ANSIColorDef(r, g, b) );

					// Custom named color combinations go here
					if ( AnsiGL_ISRGB(0, 0, 0) )
						CurColor->Name = "Black";
					else if ( AnsiGL_ISRGB(5, 0, 0) )
						CurColor->Name = "Bright Red";
					// ...

					Master->push_back( CurColor );
				}
			}
		}

// Remove temporary macro
#undef AnsiGL_ISRGB
// Remove temporary macro

		// Now for the grayscale colors
		for ( unsigned char gs = 0; gs <= 23; ++gs )
		{
			ANSIColorDef::Ptr CurColor = ANSIColorDef::Ptr( new ANSIColorDef(gs) );

			if ( gs == 0 )
				CurColor->Name = "Black";
			else if ( gs == 23 )
				CurColor->Name = "White";
			// ...

			Master->push_back( CurColor );
		}
	}

	ANSIColorDef::Ptr &ANSIColorPalette::operator[]( size_t index )
	{
		return m_Colors[index];
	}

	const ANSIColorDef::Ptr &ANSIColorPalette::operator[]( size_t index ) const
	{
		return m_Colors[index];
	}

	ANSIColorPalette::iterator ANSIColorPalette::begin()
	{
		return m_Colors.begin();
	}

	ANSIColorPalette::iterator ANSIColorPalette::end()
	{
		return m_Colors.end();
	}

	ANSIColorPalette::const_iterator ANSIColorPalette::begin() const
	{
		return m_Colors.begin();
	}

	ANSIColorPalette::const_iterator ANSIColorPalette::end() const
	{
		return m_Colors.end();
	}

	ANSIColorPalette::reverse_iterator ANSIColorPalette::rbegin()
	{
		return m_Colors.rbegin();
	}

	ANSIColorPalette::reverse_iterator ANSIColorPalette::rend()
	{
		return m_Colors.rend();
	}

	ANSIColorPalette::const_reverse_iterator ANSIColorPalette::rbegin() const
	{
		return m_Colors.rbegin();
	}

	ANSIColorPalette::const_reverse_iterator ANSIColorPalette::rend() const
	{
		return m_Colors.rend();
	}

	bool ANSIColorPalette::empty() const
	{
		return m_Colors.empty();
	}

	size_t ANSIColorPalette::size() const
	{
		return m_Colors.size();
	}

	void ANSIColorPalette::push_back( ANSIColorDef::Ptr color )
	{
		m_Colors.push_back( color );
	}

	int ANSIColorPalette::FindIndex( const ANSIColorDef &color ) const
	{
		if ( empty() )
			return -1;

		for ( unsigned int CurIndex = 0; CurIndex < size(); ++CurIndex )
		{
			if ( *(*this)[CurIndex] == color )
				return CurIndex;
		}

		return -1;
	}

	int ANSIColorPalette::FindIndex( const std::string &colorName ) const
	{
		if ( colorName.empty() || empty() )
			return -1;

		for ( unsigned int CurIndex = 0; CurIndex < size(); ++CurIndex )
		{
			if ( !colorName.compare((*this)[CurIndex]->Name) )
				return CurIndex;
		}

		return -1;
	}

	int ANSIColorPalette::FindIndex( ENUM_ANSISystemColors systemColor ) const
	{
		if ( empty() )
			return -1;

		for ( unsigned int CurIndex = 0; CurIndex < size(); ++CurIndex )
		{
			if ( (*this)[CurIndex]->IsSysColor() && (*this)[CurIndex]->SystemColor() == systemColor )
				return CurIndex;
		}

		return -1;
	}

	int ANSIColorPalette::FindIndex( unsigned char r, unsigned char g, unsigned char b ) const
	{
		if ( empty() )
			return -1;

		for ( unsigned int CurIndex = 0; CurIndex < size(); ++CurIndex )
		{
			if ( !(*this)[CurIndex]->IsRGB() )
				continue;

			unsigned char R = 0, G = 0, B = 0;
			(*this)[CurIndex]->GetRGB( &R, &G, &B );

			if ( R == r && G == g && B == b )
				return CurIndex;
		}

		return -1;
	}

	int ANSIColorPalette::FindIndex( unsigned char grayscale ) const
	{
		if ( empty() )
			return -1;

		for ( unsigned int CurIndex = 0; CurIndex < size(); ++CurIndex )
		{
			if ( (*this)[CurIndex]->IsGrayscale() && (*this)[CurIndex]->Grayscale() == grayscale )
				return CurIndex;
		}

		return -1;
	}

	ANSIColorDef::Ptr ANSIColorPalette::FindColor( const std::string &colorName ) const
	{
		for ( unsigned int CurColor = 0; CurColor < size(); ++CurColor )
		{
			if ( !colorName.compare((*this)[CurColor]->Name) )
				return (*this)[CurColor];
		}

		return ANSIColorDef::Ptr();
	}

	ANSIColorDef::Ptr ANSIColorPalette::FindColor( ENUM_ANSISystemColors systemColor ) const
	{
		for ( unsigned int CurColor = 0; CurColor < size(); ++CurColor )
		{
			if ( (*this)[CurColor]->SystemColor() == systemColor )
				return (*this)[CurColor];
		}

		return ANSIColorDef::Ptr();
	}

	ANSIColorDef::Ptr ANSIColorPalette::FindColor( unsigned char r, unsigned char g, unsigned char b ) const
	{
		unsigned char R = 0, G = 0, B = 0;

		for ( unsigned int CurColor = 0; CurColor < size(); ++CurColor )
		{
			(*this)[CurColor]->GetRGB( &R, &G, &B );

			if ( R == r && G == g && B == b )
				return (*this)[CurColor];
		}

		return ANSIColorDef::Ptr();
	}

	ANSIColorDef::Ptr ANSIColorPalette::FindColor( unsigned char grayscale ) const
	{
		for ( unsigned int CurColor = 0; CurColor < size(); ++CurColor )
		{
			if ( (*this)[CurColor]->Grayscale() == grayscale )
				return (*this)[CurColor];
		}

		return ANSIColorDef::Ptr();
	}


	std::string CLS()
	{
		std::string Str( ANSI_CODE_START );
		Str.append( "2J" );			// 2J is the VT100(IIRC) CLS code
		return Str;
	}
}


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --pad-oper


