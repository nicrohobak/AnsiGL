/*
 * ncurses.cpp
 *
 * A collection of ncurses compatibility functions.
 */


#include <clocale>
#include <map>
#include <string>

#include "../AnsiGL/ncurses/ncurses.h"
#include "../AnsiGL/ansi.h"


namespace AnsiGL
{
	ncurses::tColorPalette ncurses::ColorPalette;


	int ncurses::ColorPair( const ColorDef &color )
	{
		// The offset needed since we're a 1-based count instead of a 0-based count like size(), since ncurses reserves COLOR_PAIR(0)
		const int COLOR_PAIR_OFFSET = 1;

		// If the color is the default color, just return '0' since ncurses reserves that index specifically for this purpose
		if ( color.IsColorless() )
			return 0;

		// Try to find the color first...if it doesn't exist, then add it and return that index
		std::map< ColorDef::Ptr, int >::iterator CurColor;

		for ( CurColor = ColorPalette.begin(); CurColor != ColorPalette.end(); ++CurColor )
		{
			// If we find a match, return it
			if ( (*CurColor->first) == color )
				return CurColor->second;
		}

		// If we've already exceed the ncurses COLOR_PAIR limit, then just return the default color to show it wasn't found since we can't create a new color pair
		if ( (int)ColorPalette.size() >= COLOR_PAIRS )
			return DEFAULT_COLOR_PAIR;

		// If we get here, we didn't find the color pair...so lets create one for it and return that
		int NewColorPair = ColorPalette.size() + COLOR_PAIR_OFFSET;

		// Make a copy of this ColorDef, just so our ncurses definitions won't change unexpectedly as the original color gets modified elsewhere (this lets printed things stay printed in the same colors on-screen even if you change your working color palette)
		ColorDef::Ptr NewColor = ColorDef::Ptr( new ColorDef(color) );

		ANSIColorDef::Ptr FGColor = color.FG.Color();
		ANSIColorDef::Ptr BGColor = color.BG.Color();

		// Now, lets determine the index values to set for our ncurses COLOR_PAIR()
		int FGIndex = FGColor ? FGColor->Index() : COLOR_WHITE;
		int BGIndex = BGColor ? BGColor->Index() : COLOR_BLACK;

		// Foreground colors and background colors between 8-15 are really just the same as 0-7 with the bold attribute set
		if ( FGIndex >= 8 && FGIndex <= 15 )
			FGIndex -= 8;

		// Background colors (the system background colors) can't be bold at all...so we force them to be their non-bold counterpart instead
		if ( BGIndex >= 8 && BGIndex <= 15 )
			BGIndex -= 8;

		// Now that we've setup our ColorPalette with this color pair, we can set the actual ncurses color pair definition and our palette
		init_pair( NewColorPair, FGIndex, BGIndex );

		ColorPalette[ NewColor ] = NewColorPair;
		return NewColorPair;
	}


	void ncurses::RenderSurface( Surface::Ptr src, WINDOW *dest, Point pos, bool transparentSpaces )
	{
		if ( !src || !dest )
			return;

#if 0
		// Create a new screen buffer based on our surface size
		Surface::Ptr ScreenBuf( new Surface( Area(src->Width(), src->Height()) ) );

		// Render our surface over to that buffer (doing this step should ensure windows, etc, render their extras as well)
		src->RenderToSurface( ScreenBuf );
#else
		static Surface::Ptr ScreenBuf;

		if ( !ScreenBuf || ScreenBuf->Size() != src->Size() )
			ScreenBuf = Surface::Ptr( new Surface(Area2D(src->Size())) );
#endif // 0

		// Start things off by resetting the attributes of this window
		wattrset( dest, A_NORMAL );	// wattrset() instead of wattron()!

		tPointType CurX = 0, CurY = 0;
		achar CurChar;

		tPointType SrcWidth = src->Width();
		tPointType SrcHeight = src->Height();

		Point CurPoint;

		ANSIColorDef::Ptr FGColor;

		for ( CurY = 0; CurY < SrcHeight; ++CurY )
		{
			for ( CurX = 0; CurX < SrcWidth; ++CurX )
			{
				CurPoint.X( CurX );
				CurPoint.Y( CurY );
				CurChar = src->GetPixel( CurPoint );

				// BUG: We can try this here, but this will probably not always do what we expect...it might in most cases though...
				if ( CurChar.Bell && src->RenderBell )
					beep();

				if ( !transparentSpaces || (transparentSpaces && CurChar.IsSpace()) )
				{
					// Convert our glyph to something ncurses can understand
					wchar_t WideGlyph[CCHARW_MAX];

					if ( !CurChar.Glyph().empty() )
						mbstowcs( WideGlyph, CurChar.Glyph().c_str(), CCHARW_MAX - 1 );	// Convert our narrow UTF-8 string into a wide string
					else
					{
						WideGlyph[0] = L' ';
						WideGlyph[1] = L'\0';
					}

					cchar_t NcursesGlyph;
					//setcchar( &NcursesGlyph, WideGlyph, Attributes(CurChar), COLOR_PAIR( ColorPair(CurChar.Color) ), NULL );	// ncurses bug?  This should work, it seems like...but it definitely does not yield expected results.

					// This work-around gets much closer to the correct rendering...
					setcchar( &NcursesGlyph, WideGlyph, Attributes(CurChar), 0, NULL );
					NcursesGlyph.attr |= COLOR_PAIR( ColorPair(CurChar.Color) );

					FGColor = CurChar.Color.FG.Color();

					// BG colors can't be bold, so we only need to check for foregrounds
					if ( FGColor )
					{
						// Colors 8-15 are just colors 0-7, but with bold set...so lets mimic that here
						if ( FGColor->Index() >= 8 && FGColor->Index() <= 15 )
							NcursesGlyph.attr |= A_BOLD;
					}

					mvwadd_wch( dest, CurY, CurX, &NcursesGlyph );
				}
			}
		}

		// As we're leaving the function, reset the attributes:
		wattrset( dest, A_NORMAL );	// wattrset() instead of wattron()!
	}

	attr_t ncurses::Attributes( const achar &ach )
	{
		attr_t Attributes = A_NORMAL;

		if ( ach.Attributes().empty() )
			return Attributes;

		// Render the attributes
		if ( ach.Attributes().Has(ANSI_BoldOn) )
			Attributes |= A_BOLD;

		if ( ach.Attributes().Has(ANSI_FaintOn) )
			Attributes |= A_DIM;

		if ( ach.Attributes().Has(ANSI_UnderlineOn) )
			Attributes |= A_UNDERLINE;

		if ( ach.Attributes().Has(ANSI_BlinkSlow) || ach.Attributes().Has(ANSI_BlinkFast) )
			Attributes |= A_BLINK;

		if ( ach.Attributes().Has(ANSI_InverseOn) )
			Attributes |= A_REVERSE;

		if ( ach.Attributes().Has(ANSI_ConcealOn) )
			Attributes |= A_PROTECT;

		if ( ach.Attributes().Has(ANSI_BoldOff) )
			Attributes &= ~(A_BOLD);

		if ( ach.Attributes().Has(ANSI_NormalIntensity) )
		{
			Attributes &= ~(A_DIM);
			Attributes &= ~(A_BOLD);
		}

		if ( ach.Attributes().Has(ANSI_UnderlineOff) )
			Attributes &= ~(A_UNDERLINE);

		if ( ach.Attributes().Has(ANSI_InverseOff) )
			Attributes &= ~(A_REVERSE);

		if ( ach.Attributes().Has(ANSI_ConcealOff) )
			Attributes &= ~(A_PROTECT);

		if ( ach.Attributes().Has(ANSI_Default) )
			Attributes = A_NORMAL;

		return Attributes;
	}
}


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --pad-oper


