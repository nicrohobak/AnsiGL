/*
 * text.cpp
 *
 * A content object that represents a text field.
 */


#include "AnsiGL/text.h"


namespace AnsiGL
{
	const astring &Text::Value() const
	{
		return _Text;
	}

	const astring &Text::FormattedValue() const
	{
		return _FormattedText;
	}

	void Text::Value( const astring &text )
	{
		_Text = text;
		format();
	}

	void Text::Value( const ustring &text )
	{
		_Text = astring( text );
		format();
	}

	void Text::Value( const std::string &text )
	{
		_Text = astring( text );
		format();
	}

	void Text::Append( const astring &text )
	{
		_Text.append( text );
	}

	void Text::Append( const ustring &text )
	{
		_Text.append( astring(text) );
	}

	void Text::Append( const std::string &text )
	{
		_Text.append( astring(text) );
	}

	void Text::Clear()
	{
		_Text.clear();
	}

	ENUM_TxtAlign Text::Align() const
	{
		return _Alignment;
	}

	void Text::Align( ENUM_TxtAlign align )
	{
		_Alignment = align;
		format();
	}

	const tSizeType Text::Width() const
	{
		return Content::Width();
	}

	void Text::Width( tSizeType width )
	{
		Content::Width( width );
		format();
	}

	const tSizeType Text::Height() const
	{
		return Content::Height();
	}

	void Text::Height( tSizeType height )
	{
		Content::Height( height );
	}

	void Text::Resize( const Area2D &size )
	{
		Content::Resize( size );
		format();
	}

	std::string Text::str()
	{
		return _FormattedText.str();
	}

	std::string Text::Render() const
	{
		return _FormattedText.Render( ColorDepth_Default );
	}

	void Text::RenderToSurface( Surface::Ptr dest, const Point2D &pos ) const
	{
		if ( !dest )
			return;

		dest->DrawString( _FormattedText, pos, TransparentSpaces );
	}

	void Text::format()
	{
		_FormattedText = _Text.Format( _Size.Width(), _Alignment );

		// If AutoHeight is on, adjust our height based on the number of newlines we have
		if ( AutoHeight )
		{
			astring::const_iterator CurChar;
			tSizeType NumLines = 1;

			for ( CurChar = _FormattedText.begin(); CurChar != _FormattedText.end(); ++CurChar )
			{
				if ( (*CurChar) == '\n' )
					++NumLines;
			}

			Content::Height( NumLines );
		}
	}
}


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --pad-oper


