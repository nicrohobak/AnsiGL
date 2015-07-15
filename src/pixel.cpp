/*
 * pixel.cpp
 *
 * Defines "Pixels" for use in a achar-based "Screen"
 */


#include <sstream>

#include "AnsiGL/pixel.h"


namespace AnsiGL
{
    Pixel::Pixel():
	PaletteIndex(-1),
	Bell(false)
    {
    }

    ANSICodeList Pixel::Attributes() const
    {
	return m_Attributes;
    }

    bool Pixel::HasAttribute( ENUM_ANSICodes attrib ) const
    {
	return m_Attributes.Has( attrib );
    }

    void Pixel::AddAttribute( ENUM_ANSICodes attrib )
    {
	// Don't allow foreground colors
	if ( attrib >= ANSI_FG_Black && attrib <= ANSI_FG_Default )
	    return;

	// And don't allow background colors either
	if ( attrib >= ANSI_BG_Black && attrib <= ANSI_BG_Default )
	    return;

	m_Attributes.Add( attrib );
    }

    void Pixel::AddAttributes( ANSICodeList attributes )
    {
	m_Attributes.Add( attributes );
    }

    void Pixel::RemoveAttribute( ENUM_ANSICodes attrib )
    {
	m_Attributes.Remove( attrib );
    }

    void Pixel::Clear()
    {
	uchar::Clear();
	m_Attributes.clear();
	PaletteIndex = 0;
	Bell = false;
    }
}


