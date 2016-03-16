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
		return _Attributes;
	}

	bool Pixel::HasAttribute( ENUM_ANSICodes attrib ) const
	{
		return _Attributes.Has( attrib );
	}

	void Pixel::AddAttribute( ENUM_ANSICodes attrib )
	{
		// Don't allow foreground colors
		if ( attrib >= ANSI_FG_Black && attrib <= ANSI_FG_Default )
			return;

		// And don't allow background colors either
		if ( attrib >= ANSI_BG_Black && attrib <= ANSI_BG_Default )
			return;

		_Attributes.Add( attrib );
	}

	void Pixel::AddAttributes( ANSICodeList attributes )
	{
		_Attributes.Add( attributes );
	}

	void Pixel::RemoveAttribute( ENUM_ANSICodes attrib )
	{
		_Attributes.Remove( attrib );
	}

	void Pixel::Clear()
	{
		uchar::Clear();
		_Attributes.clear();
		PaletteIndex = 0;
		Bell = false;
	}
}


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --pad-oper


