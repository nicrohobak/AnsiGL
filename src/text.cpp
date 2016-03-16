/*
 * text.cpp
 *
 * A content object that represents a text field.
 */


#include "AnsiGL/text.h"


namespace AnsiGL
{
	Text::Text():
		m_Text("Default Text"),
		m_Alignment(TxtAlign_Default),
		AutoHeight(true),
		TransparentSpaces(false)
	{
		Resize( Area2D(12, 1) );		// Size for "Default Text"
	}

	Text::Text( const astring &text, tSizeType width, ENUM_TxtAlign alignment, bool transparentSpaces, bool autoHeight ):
		m_Text(text),
		m_Alignment(alignment),
		AutoHeight(autoHeight),
		TransparentSpaces(transparentSpaces)
	{
		if ( width == 0 )
			Width( text.length() );
		else
			Width( width );
	}

	Text::~Text()
	{
	}

	const astring &Text::Value() const
	{
		return m_Text;
	}

	const astring &Text::FormattedValue() const
	{
		return m_FormattedText;
	}

	void Text::Value( const astring &text )
	{
		m_Text = text;
		format();
	}

	void Text::Value( const ustring &text )
	{
		m_Text = astring( text );
		format();
	}

	void Text::Value( const std::string &text )
	{
		m_Text = astring( text );
		format();
	}

	void Text::Append( const astring &text )
	{
		m_Text.append( text );
	}

	void Text::Append( const ustring &text )
	{
		m_Text.append( astring(text) );
	}

	void Text::Append( const std::string &text )
	{
		m_Text.append( astring(text) );
	}

	void Text::Clear()
	{
		m_Text.clear();
	}

	ENUM_TxtAlign Text::Align() const
	{
		return m_Alignment;
	}

	void Text::Align( ENUM_TxtAlign align )
	{
		m_Alignment = align;
		format();
	}

	const tSizeType &Text::Width() const
	{
		return Content::Width();
	}

	void Text::Width( tSizeType width )
	{
		Content::Width( width );
		format();
	}

	const tSizeType &Text::Height() const
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
		return m_FormattedText.str();
	}

	std::string Text::Render() const
	{
		return m_FormattedText.Render( ColorDepth_Default );
	}

	void Text::RenderToSurface( Surface::Ptr dest, const Point2D &pos ) const
	{
		if ( !dest )
			return;

		dest->DrawString( m_FormattedText, pos, TransparentSpaces );
	}

	void Text::format()
	{
		m_FormattedText = m_Text.Format( m_Size.Width(), m_Alignment );

		// If AutoHeight is on, adjust our height based on the number of newlines we have
		if ( AutoHeight )
		{
			astring::const_iterator CurChar;
			tSizeType NumLines = 1;

			for ( CurChar = m_FormattedText.begin(); CurChar != m_FormattedText.end(); ++CurChar )
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


