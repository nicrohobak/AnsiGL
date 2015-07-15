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
	ANSIColorPalette::Ptr	m_Palette;		// A pointer to the current ANSIColorPalette
	int			m_Index;		// The color index of this component: Range: -1 to 255 (typically)  -1 is colorless

    public:
	ColorComponent();
	ColorComponent( ANSIColorPalette::Ptr palette );
	ColorComponent( ANSIColorPalette::Ptr palette, const ANSIColorDef &color );
	ColorComponent( ENUM_ANSISystemColors color );
	ColorComponent( unsigned char r, unsigned char g, unsigned char b );
	ColorComponent( unsigned char grayscale );

	bool operator==( const ColorComponent &right ) const;
	bool operator!=( const ColorComponent &right ) const;

	ANSIColorPalette::Ptr Palette() const;
	void Palette( ANSIColorPalette::Ptr palette );

	int Index() const;

	void Set( ENUM_ANSISystemColors color );
	void Set( unsigned char r, unsigned char g, unsigned char b );
	void Set( unsigned char grayscale );

	ANSIColorDef::Ptr Color() const;		// Returns the ANSIColorDef for this color (if it has one)

	bool IsColorless() const;			// Returns true if this color component is colorless

	void Clear();

	std::string Render( ENUM_ColorDepth desiredDepth, bool background = false ) const;	// If background is true, this color is rendered as a background color

    protected:
	void assignMasterPalette();			// Initializes AnsiGL's master color palette: ANSIColorPalette::Master, if necessary then points m_Palette over to it
    };


    //
    // ColorDef is how AnsiGL::achar gets its color
    //
    class ColorDef
    {
    public:
	typedef boost::shared_ptr< ColorDef >	Ptr;

    public:
	ColorComponent		FG;
	ColorComponent		BG;

	bool			Inverted;		// Invert the foreground and background colors when rendering

    public:
	ColorDef();
	ColorDef( const ColorComponent &fg, const ColorComponent &bg );
	ColorDef( const ColorComponent &fg, const ColorComponent &bg, bool inverted );
	ColorDef( ENUM_ANSISystemColors fg, ENUM_ANSISystemColors bg );

	bool operator==( const ColorDef &right ) const;	// Returns true if they are effectively equivalant colors, even if not constructed identically or named identically
	bool operator!=( const ColorDef &right ) const;

	bool IsColorless() const;

	void SetPalette( ANSIColorPalette::Ptr palette );	// Assigns this palette to the FG and BG colors

	void Clear();

	std::string Render( ENUM_ColorDepth desiredDepth = ColorDepth_7Bit ) const;	// Renders the ANSI color code for this color...no escape sequence, only numbers and separators.
    };


    //
    // ColorPalletes are the common user-interface to the palette system
    //
    class ColorPalette
    {
    public:
	typedef boost::shared_ptr< ColorPalette >			Ptr;

	typedef std::list< ColorDef::Ptr >::iterator			iterator;
	typedef std::list< ColorDef::Ptr >::const_iterator		const_iterator;
	typedef std::list< ColorDef::Ptr >::reverse_iterator		reverse_iterator;
	typedef std::list< ColorDef::Ptr >::const_reverse_iterator	const_reverse_iterator;

    protected:
	std::list< ColorDef::Ptr >		m_Colors;

    public:
	ColorPalette();

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
    };
}




#endif // __ANSIGL_COLOR_H__




