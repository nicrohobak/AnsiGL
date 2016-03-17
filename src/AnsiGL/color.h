#ifndef __ANSIGL_COLOR_H__
#define __ANSIGL_COLOR_H__

/*
 * color.h
 *
 * ANSI and colors and palettes, oh my!
 */


#include "ansi.h"


namespace AnsiGL
{
	//
	// One component of our color color definition
	//
	class ColorComponent
	{
	protected:
		ANSIColorPalette::Ptr	_Palette;		// A pointer to the current ANSIColorPalette
		int						_Index;			// The color index of this component: Range: -1 to 255 (typically)  -1 is colorless

	public:
		ColorComponent():
			_Index(-1)
		{
			assignMasterPalette();
		}

		ColorComponent( ANSIColorPalette::Ptr palette ):
			_Palette(palette),
			_Index(-1)
		{
			// This is one of the only 2 constructors that do NOT initialize the ANSIColorPalette::Master!
		}

		ColorComponent( ANSIColorPalette::Ptr palette, const ANSIColorDef &color ):
			_Palette(palette),
			_Index(-1)
		{
			// This is one of the only 2 constructors that do NOT initialize the ANSIColorPalette::Master!

			if ( _Palette )
				_Index = _Palette->FindIndex( color );
		}

		ColorComponent( ENUM_ANSISystemColors color ):
			_Index(-1)
		{
			assignMasterPalette();
			Set( color );
		}

		ColorComponent( unsigned char r, unsigned char g, unsigned char b ):
			_Index(-1)
		{
			assignMasterPalette();
			Set( r, g, b );
		}

		ColorComponent( unsigned char grayscale ):
			_Index(-1)
		{
			assignMasterPalette();
			Set( grayscale );
		}

		bool operator==( const ColorComponent &right ) const;
		bool operator!=( const ColorComponent &right ) const;

		ANSIColorPalette::Ptr Palette() const
		{
			return _Palette;
		}

		void Palette( ANSIColorPalette::Ptr palette );

		int Index() const
		{
			return _Index;
		}

		void Set( ENUM_ANSISystemColors color );
		void Set( unsigned char r, unsigned char g, unsigned char b );
		void Set( unsigned char grayscale );

		ANSIColorDef::Ptr Color() const;		// Returns the ANSIColorDef for this color (if it has one)

		bool IsColorless() const				// Returns true if this color component is colorless
		{
			return (!_Palette || _Index == -1);
		}

		void Clear()
		{
			_Palette.reset();
			_Index = -1;
		}

		std::string Render( ENUM_ColorDepth desiredDepth, bool background = false ) const;	// If background is true, this color is rendered as a background color

	protected:
		void assignMasterPalette();				// Initializes AnsiGL's master color palette: ANSIColorPalette::Master, if necessary then points _Palette over to it
	};


	//
	// ColorDef is how AnsiGL::achar gets its color
	//
	class ColorDef
	{
	public:
		ANSIGL_POINTERS( ColorDef )

	public:
		ColorComponent		FG;
		ColorComponent		BG;

		bool			Inverted;		// Invert the foreground and background colors when rendering

	public:
		ColorDef():
			Inverted(false)
		{
		}

		ColorDef( const ColorComponent &fg, const ColorComponent &bg ):
			FG(fg),
			BG(bg),
			Inverted(false)
		{
		}

		ColorDef( const ColorComponent &fg, const ColorComponent &bg, bool inverted ):
			FG(fg),
			BG(bg),
			Inverted(inverted)
		{
		}

		ColorDef( ENUM_ANSISystemColors fg, ENUM_ANSISystemColors bg ):
			FG(fg),
			BG(bg),
			Inverted(false)
		{
		}

		bool operator==( const ColorDef &right ) const;	// Returns true if they are effectively equivalant colors, even if not constructed identically or named identically
		bool operator!=( const ColorDef &right ) const;

		bool IsColorless() const
		{
			return (FG.IsColorless() && BG.IsColorless());
		}

		void SetPalette( ANSIColorPalette::Ptr palette );	// Assigns this palette to the FG and BG colors

		void Clear()
		{
			FG.Clear();
			BG.Clear();
			Inverted = false;
		}

		std::string Render( ENUM_ColorDepth desiredDepth = ColorDepth_7Bit ) const;	// Renders the ANSI color code for this color...no escape sequence, only numbers and separators.
	};


	//
	// ColorPalletes are the common user-interface to the palette system
	//
	class ColorPalette
	{
	public:
		ANSIGL_POINTERS( ColorPalette )

		typedef std::list< ColorDef::Ptr >::iterator			iterator;
		typedef std::list< ColorDef::Ptr >::const_iterator		const_iterator;
		typedef std::list< ColorDef::Ptr >::reverse_iterator		reverse_iterator;
		typedef std::list< ColorDef::Ptr >::const_reverse_iterator	const_reverse_iterator;

	public:
		ColorPalette()
		{
		}

		ColorDef::Ptr operator[]( int index );
		const ColorDef::Ptr operator[]( int index ) const;

		iterator begin();
		iterator end();
		const_iterator begin() const;
		const_iterator end() const;
		reverse_iterator rbegin();
		reverse_iterator rend();
		const_reverse_iterator rbegin() const;
		const_reverse_iterator rend() const;

		bool empty() const;
		size_t size() const;

		void push_back( ANSIColorDef::Ptr color );

		// These functions should be favored over any in std::list<>!
		int Add( ColorDef::Ptr color );			// Effectively just push_back() that returns the index of the added color
		void Remove( const ColorDef &color );
		void Remove( int index );

		int FindIndex( const ColorDef &color ) const;

		//
		// Special Effects
		//
		void InvertColors();				// Inverts all colors in the palette, FG/BG color swap

		// shift operators could move the palette colors
		// increment/decrement operators could do the same

	protected:
		std::list< ColorDef::Ptr >		_Colors;
	};
}


#endif // __ANSIGL_COLOR_H__


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --pad-oper


