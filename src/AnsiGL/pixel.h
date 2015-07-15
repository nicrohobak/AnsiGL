#ifndef __ANSIGL_PIXEL_H__
#define __ANSIGL_PIXEL_H__

/*
 * pixel.h
 *
 * Defines ANSI "pixels" for an AnsiGL::Surface
 * This is effectively identical to AnsiGL::achar, except for the color palette!...Unfortunately, that means it should primarily (if not exclusively) be manipulated by the containing screen (otherwise the pixel has no clue about its own palette!)
 */


#include "ansi.h"
#include "uchar.h"


namespace AnsiGL
{
    class Pixel : public uchar
    {
    protected:
	ANSICodeList	m_Attributes;		// Except color!

    public:
	int		PaletteIndex;
	bool		Bell;

    public:
	Pixel();

	ANSICodeList Attributes() const;

	bool HasAttribute( ENUM_ANSICodes attrib ) const;

	void AddAttribute( ENUM_ANSICodes attrib );
	void AddAttributes( ANSICodeList attributes );
	void RemoveAttribute( ENUM_ANSICodes attrib );

	void Clear();

	friend class Surface;	// For fast access
    };
}

#endif // __ANSIGL_PIXEL_H__


