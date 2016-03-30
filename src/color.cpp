/*
 * color.cpp
 *
 * ANSI and colors and palettes, oh my!
 */


#include <sstream>

#include "AnsiGL/color.h"


namespace AnsiGL
{
	bool ColorComponent::operator==( const ColorComponent &right ) const
	{
		if ( this == &right )
			return true;

		if ( _Index == -1 && right._Index == -1 )
			return true;

		if ( _Palette && right._Palette )
		{
			if ( _Index >= 0 && _Index < (int)_Palette->size() && right._Index >= 0 && right._Index < (int)right._Palette->size() )
			{
				// _Palette is a pointer, and so is (*_Palette)[index]...hence the double deref
				if ( (*(*_Palette)[_Index]) == (*(*right._Palette)[right._Index]) )
					return true;
			}
		}

		return false;
	}

	bool ColorComponent::operator!=( const ColorComponent &right ) const
	{
		return !((*this) == right);
	}

	void ColorComponent::Palette( ANSIColorPalette::Ptr palette )
	{
		_Palette = palette;

		// Check our index bounds against the new palette...if we're still in range, allow it...if not, become transparent
		if ( !_Palette || (_Palette && _Index >= (int)_Palette->size()) )
			_Index = -1;
	}

	void ColorComponent::Set( ENUM_ANSISystemColors color )
	{
		if ( !_Palette )
			return;

		_Index = _Palette->FindIndex( color );
	}

	void ColorComponent::Set( unsigned char r, unsigned char g, unsigned char b )
	{
		if ( !_Palette )
			return;

		_Index = _Palette->FindIndex( r, g, b );
	}

	void ColorComponent::Set( unsigned char grayscale )
	{
		if ( !_Palette )
			return;

		_Index = _Palette->FindIndex( grayscale );
	}

	ANSIColorDef::Ptr ColorComponent::Color() const
	{
		//if ( _Palette && _Index != -1 && _Index < _Palette->size() )
		if ( _Palette && _Index != -1 )
			return (*_Palette)[_Index];

		return ANSIColorDef::Ptr();
	}

	std::string ColorComponent::Render( ENUM_ColorDepth desiredDepth, bool background ) const
	{
		if ( IsColorless() || _Index >= (int)_Palette->size() )
			return std::string("");

		return (*_Palette)[_Index]->Render( desiredDepth, background );
	}

	void ColorComponent::initMasterPalette()
	{
		if ( !ANSIColorPalette::Master )
			ANSIColorPalette::InitMasterPalette();
	}

	void ColorComponent::assignMasterPalette()
	{
		initMasterPalette();
		_Palette = ANSIColorPalette::Master;
	}


	bool ColorDef::operator==( const ColorDef &right ) const
	{
		if ( Inverted )
		{
			if ( right.Inverted )
			{
				// Us inverted, them inverted
				if ( BG == right.BG && FG == right.FG )
					return true;
			}
			else
			{
				// Us inverted, them not
				if ( BG == right.FG && FG == right.BG )
					return true;
			}
		}
		else
		{
			if ( right.Inverted )
			{
				// Them inverted, us not
				if ( FG == right.BG && BG == right.FG )
					return true;
			}
			else
			{
				// Nobody inverted
				if ( FG == right.FG && BG == right.BG )
					return true;
			}
		}

		return false;
	}

	bool ColorDef::operator!=( const ColorDef &right ) const
	{
		return !((*this) == right);
	}

	void ColorDef::SetPalette( ANSIColorPalette::Ptr palette )
	{
		if ( !palette )
			return;

		FG.Palette( palette );
		BG.Palette( palette );
	}

	std::string ColorDef::Render( ENUM_ColorDepth desiredDepth ) const
	{
		std::stringstream RenderedStr("");

		if ( IsColorless() )
			return RenderedStr.str();

		if ( Inverted )
		{
			RenderedStr << BG.Render( desiredDepth );

			// Add a separator if needed
			if ( !FG.IsColorless() && !BG.IsColorless() )
				RenderedStr << ANSI_CODE_SEPARATOR;

			RenderedStr << FG.Render( desiredDepth, true );	// The true 2nd parameter renders this as a background color instead of as a foreground color
		}
		else
		{
			RenderedStr << FG.Render( desiredDepth );

			// Add a separator if needed
			if ( !FG.IsColorless() && !BG.IsColorless() )
				RenderedStr << ANSI_CODE_SEPARATOR;


			RenderedStr << BG.Render( desiredDepth, true );	// The true 2nd parameter renders this as a background color instead of as a foreground color
		}

		return RenderedStr.str();
	}


	ColorDef::Ptr ColorPalette::operator[]( int index )
	{
		if ( empty() || index < 0 || index >= (int)size() )
			return ColorDef::Ptr();

		// If the index we want is greater than 1/2 of the total palette size...
		if ( index > ((int)size() >> 1) )
		{
			// Start from the back
			ColorPalette::reverse_iterator CurColor;
			int CurIndex = (int)size() - 1;

			for ( CurColor = rbegin(); CurColor != rend(); ++CurColor, --CurIndex )
			{
				if ( CurIndex == index )
					return (*CurColor);
			}
		}
		else
		{
			// Start from the front
			ColorPalette::iterator CurColor;
			int CurIndex = 0;

			for ( CurColor = begin(); CurColor != end(); ++CurColor, ++CurIndex )
			{
				if ( CurIndex == index )
					return (*CurColor);
			}
		}

		return ColorDef::Ptr();
	}

	const ColorDef::Ptr ColorPalette::operator[]( int index ) const
	{
		if ( empty() || index < 0 || index >= (int)size() )
			return ColorDef::Ptr();

		// If the index we want is greater than 1/2 of the total palette size...
		if ( index > ((int)size() >> 1) )
		{
			// Start from the back
			ColorPalette::const_reverse_iterator CurColor;
			int CurIndex = (int)size() - 1;

			for ( CurColor = rbegin(); CurColor != rend(); ++CurColor, --CurIndex )
			{
				if ( CurIndex == index )
					return (*CurColor);
			}
		}
		else
		{
			// Start from the front
			ColorPalette::const_iterator CurColor;
			int CurIndex = 0;

			for ( CurColor = begin(); CurColor != end(); ++CurColor, ++CurIndex )
			{
				if ( CurIndex == index )
					return (*CurColor);
			}
		}

		return ColorDef::Ptr();
	}

	ColorPalette::iterator ColorPalette::begin()
	{
		return _Colors.begin();
	}

	ColorPalette::iterator ColorPalette::end()
	{
		return _Colors.end();
	}

	ColorPalette::const_iterator ColorPalette::begin() const
	{
		return _Colors.begin();
	}

	ColorPalette::const_iterator ColorPalette::end() const
	{
		return _Colors.end();
	}

	ColorPalette::reverse_iterator ColorPalette::rbegin()
	{
		return _Colors.rbegin();
	}

	ColorPalette::reverse_iterator ColorPalette::rend()
	{
		return _Colors.rend();
	}

	ColorPalette::const_reverse_iterator ColorPalette::rbegin() const
	{
		return _Colors.rbegin();
	}

	ColorPalette::const_reverse_iterator ColorPalette::rend() const
	{
		return _Colors.rend();
	}

	bool ColorPalette::empty() const
	{
		return _Colors.empty();
	}

	size_t ColorPalette::size() const
	{
		return _Colors.size();
	}

	int ColorPalette::Add( ColorDef::Ptr color )
	{
		// If it's colorless, do nothing
		if ( color && color->IsColorless() )
			return -1;

		// Then try and find the color first...
		int NewIndex = FindIndex( *color );

		// If we found something, use that color rather than adding a duplicate
		if ( NewIndex != -1 )
			return NewIndex;

		// Otherwise, lets actually add the color
		NewIndex = size();		// The new index will be whatever the current size is, since we count from base 0 with our index
		_Colors.push_back( color );

		return NewIndex;
	}

	void ColorPalette::Remove( const ColorDef &color )
	{
		if ( empty() )
			return;

		ColorPalette::iterator CurColor = begin();

		while ( CurColor != end() )
		{
			// CurColor is an iterator to ColorDef::Ptr...hence the double-dref
			if ( (*(*CurColor)) == color )
				CurColor = _Colors.erase( CurColor );
			else
				++CurColor;
		}
	}

	void ColorPalette::Remove( int index )
	{
		if ( index <= -1 || index >= (int)size() )
			return;

		if ( empty() )
			return;

		// If the index we want is greater than 1/2 of the total palette size...
		if ( index > ((int)size() >> 1) )
		{
			// Start from the back
			ColorPalette::reverse_iterator CurColor;
			int CurIndex = size() - 1;

			for ( CurColor = rbegin(); CurColor != rend(); ++CurColor, --CurIndex )
			{
				if ( CurIndex == index )
				{
					_Colors.erase( --CurColor.base() );
					break;
				}
			}
		}
		else
		{
			// Start from the front
			ColorPalette::iterator CurColor;
			int CurIndex = 0;

			for ( CurColor = begin(); CurColor != end(); ++CurColor, ++CurIndex )
			{
				if ( CurIndex == index )
				{
					_Colors.erase( CurColor );
					break;
				}
			}
		}
	}

	int ColorPalette::FindIndex( const ColorDef &color ) const
	{
		if ( empty() )
			return -1;

		ColorPalette::const_iterator CurColor;
		int ColorIndex = 0;

		for ( CurColor = begin(); CurColor != end(); ++CurColor, ++ColorIndex )
		{
			// CurColor is an iterator to ColorDef::Ptr...hence the double-dref
			if ( (*(*CurColor)) == color )
				return ColorIndex;
		}

		return -1;
	}

	void ColorPalette::InvertColors()
	{
		ColorPalette::iterator CurColor;

		for ( CurColor = begin(); CurColor != end(); ++CurColor )
			(*CurColor)->Inverted = !(*CurColor)->Inverted;
	}

	/*
	    std::string ColorPalette::Render()
	    {
			std::stringstream RenderedStr("");

			ColorPalette::iterator CurColor;

			RenderedStr << "\033[" << ANSI_Default;		// ANSI Code Begin and reset all ANSI attributes

			for ( CurColor = begin(); CurColor != end(); ++CurColor )
			    RenderedStr << ';' << CurColor->Render();

			RetStr << 'm';		// ANSI Code End

			return RenderedStr.str();
	    }
	*/
}


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --indent-switches --pad-oper


