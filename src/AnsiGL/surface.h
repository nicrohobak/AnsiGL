#ifndef __ANSIGL_SURFACE_H__
#define __ANSIGL_SURFACE_H__

/*
 * surface.h
 *
 * A rendering context for ANSI-enabled ASCII "graphics".
 */


#include <string>
#include <vector>

#include "achar.h"
#include "area.h"
#include "pixel.h"
#include "string.h"


namespace AnsiGL
{
	//
	// A variable, just in case we want to easily change this behavior later
	//
	const bool TRANSPARENT_DEFAULT = false;


	//
	// typedef to create our Scanline data type
	//
	typedef std::vector< Pixel >	Scanline;


	//
	// The basis of all rendering targets in this library beyond astrings themselves
	//
	class Surface
	{
	public:
		ANSIGL_POINTERS( Surface )

	public:
		ColorPalette::Ptr		Palette;	// The color palette for this surface
		bool				AutoPalette;	// On by default, determines if this surface will manage its own palette automatically

		ENUM_ColorDepth			DesiredDepth;	// 7Bit by default (16 foreground, 8 background colors)

		int				TabSize;	// 8 by default, the number of spaces a tab character should represent (for std::strings drawn vertically, tabs are 1/4 this size, with a minimum of 1 space).  8 seems to be the tab size of most consoles I've encountered.

		bool				RenderANSI;	// On by default
		bool				RenderBell;	// On by default (The bell character, '\007')

		bool				UseCLS;		// Off by default, adds an ANSI clear screen code to the start of the surface rendering

		bool				UseENDL;	// On by default, adds a endl character to the end of each line when rendering
		bool				UseLF;		// Off by default, adds a '\n' at the end of each line when rendering
		bool				UseCR;		// Off by default, adds a '\r' at the end of each line when rendering

	public:
		Surface();
		Surface( const Area2D &size );

		virtual ~Surface();

		// This is a little safer than direct access to our vector...
		// This is preferred unless direct access is absolutely necessary (usually only for low-level rendering functions, etc.)
		Scanline GetScanline( int lineNum ) const;
		const achar &GetPixel( const Point2D &pixel ) const;

		virtual const Area2D &Size() const;
		virtual void Resize( Area2D size );

		virtual const tSizeType &Width() const;
		virtual void Width( tSizeType width );

		virtual const tSizeType &Height() const;
		virtual void Height( tSizeType height );

		void Clear();

		bool HasPoint( const Point2D &pos ) const	// Returns true if this coordinate is within the bounds of this screen
		{
			return (pos.X() >= 0 && pos.X() < m_Size.Width() && pos.Y() >= 0 && pos.Y() < m_Size.Height());
		}

		void AddToPalette( ColorPalette::Ptr palette );	// Adds the contents of another color palette to ours (avoiding duplicate entries)
		int FindColorIndex( const ColorDef &color );	// Finds the index of a color in our palette (adding the color to the palette if AutoPalette is on and it isn't already there)
		ColorDef::Ptr FindColorFromIndex( int index ) const;	// Finds the color definition based on the provided index

		//
		// Render Functions
		//
		virtual std::string str();			// Calls Render() with ANSI off by default instead of on
		virtual std::string Render() const;		// Converts this surface into a std::string
		virtual void RenderToSurface( Surface::Ptr dest, const Point2D &point = Point2D(), bool transparentSpaces = TRANSPARENT_DEFAULT ) const;	// Renders this surface to another surface
		virtual void RenderAreaToSurface( FixedArea2D visibleArea, Surface::Ptr dest, const Point2D &point = Point2D(), bool transparentSpaces = TRANSPARENT_DEFAULT ) const;	// Renders an area on this surface to another surface


		//
		// Draw*() Functions
		//
		// Each has a pos (position) parameter.  This is an offset from the origin (0,0) of the screen.
		// Each has a transparentSpaces parameter.  When set, any space characters do not overwrite the previous character at that position.
		// DrawString (and related) draw relative to the position parameter and do not attempt to wrap.  If they exceed the bounds of the screen, they will simply trunkate.
		//
		virtual void DrawChar( const achar &ach, const Point2D &point = Point2D(), bool transparentSpaces = TRANSPARENT_DEFAULT );
		virtual void DrawChar( const std::string &ch, const Point2D &point = Point2D(), bool transparentSpaces = TRANSPARENT_DEFAULT );

		//
		// Strings...it's kinda like drawing lines
		//
		virtual void DrawString( const astring &str, const Point2D &pos = Point2D(), bool transparentSpaces = TRANSPARENT_DEFAULT, bool invertString = false, bool rightToLeft = false, bool invertNewlineDir = false );
		virtual void DrawString( const std::string &str, const Point2D &pos = Point2D(), bool transparentSpaces = TRANSPARENT_DEFAULT, bool invertString = false, bool rightToLeft = false, bool invertNewlineDir = false );

		virtual void DrawVString( const astring &str, const Point2D &pos = Point2D(), bool transparentSpaces = TRANSPARENT_DEFAULT, bool invertString = false, bool bottomToTop = false, bool invertNewlineDir = false );
		virtual void DrawVString( const std::string &str, const Point2D &po = Point2D(), bool transparentSpaces = TRANSPARENT_DEFAULT, bool invertString = false, bool bottomToTop = false, bool invertNewlineDir = false );

		//
		// Extended drawing functions, boxes, etc.
		//
		virtual void FillGlyph( const Area2D &size, const uchar &glyph = uchar(), const Point2D &pos = Point2D() );
		virtual void FillColor( const Area2D &size, const ColorDef &color = ColorDef(), const Point2D &pos = Point2D() );
		virtual void FillChar( const Area2D &size, const achar &ach = achar(), const Point2D &pos = Point2D() );

		virtual void DrawBox( const Area2D &box, const Point2D &pos = Point2D(), const achar &hBorder = achar("-"), const achar &vBorder = achar("|"), const achar &fillCh = achar(" "), bool transparentSpaces = TRANSPARENT_DEFAULT );

	protected:
		std::vector< Scanline >		m_Pixels;	// NOTE: Direct access is via y,x (m_Pixels[y][x]) rather than x,y!

		Area2D				m_Size;

	protected:
		bool pixelHasAnsi( const Point2D &pixel ) const;
		std::string pixelRenderAnsi( const Point2D &pixel ) const;
	};


	std::ostream &operator<<( std::ostream &left, const Surface &right );
}


#endif // __ANSIGL_SURFACE_H__


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --pad-oper


