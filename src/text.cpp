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
		this->format();
	}

	void Text::Value( const ustring &text )
	{
		_Text = astring( text );
		this->format();
	}

	void Text::Value( const std::string &text )
	{
		_Text = astring( text );
		this->format();
	}

	void Text::Append( const astring &text )
	{
		_Text.append( text );
		this->format();
	}

	void Text::Append( const ustring &text )
	{
		_Text.append( astring(text) );
		this->format();
	}

	void Text::Append( const std::string &text )
	{
		_Text.append( astring(text) );
		this->format();
	}

	void Text::Append( const achar &ch )
	{
		_Text.push_back( ch );
		this->format();
	}

	bool Text::empty() const
	{
		return _Text.empty();
	}

	void Text::push_back( const achar &ch )
	{
		Append( ch );
	}

	void Text::pop_back()
	{
		if ( _Text.empty() )
			return;

		_Text.pop_back();
		this->format();
	}

	void Text::Clear()
	{
		_Text.clear();
		_FormattedText.clear();

		if ( AutoHeight )
			Content::Height( 0 );
	}

	ENUM_TxtAlign Text::Align() const
	{
		return _Alignment;
	}

	void Text::Align( ENUM_TxtAlign align )
	{
		_Alignment = align;
		this->format();
	}

	tSizeType Text::Length() const
	{
		return _Text.length();
	}

	const tSizeType Text::Width() const
	{
		return Content::Width();
	}

	void Text::Width( tSizeType width )
	{
		Content::Width( width );
		this->format();
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
		this->format();
	}

	std::string Text::str()
	{
		return _FormattedText.str();
	}

	std::string Text::Render() const
	{
		return _FormattedText.Render( _ColorDepth );
	}

	void Text::RenderToSurface( Surface::Ptr dest, const Point2D &pos ) const
	{
		if ( !dest )
			return;

		dest->DrawString( _FormattedText, pos, TransparentSpaces );
	}

	Text &Text::operator<<( void *val )
	{
		return addToStream< void * >( val );
	}

	Text &Text::operator<<( bool val )
	{
		return addToStream< bool >( val );
	}

	Text &Text::operator<<( char val )
	{
		return addToStream< char >( val );
	}

	Text &Text::operator<<( unsigned char val )
	{
		return addToStream< unsigned char >( val );
	}

	Text &Text::operator<<( short val )
	{
		return addToStream< short >( val );
	}

	Text &Text::operator<<( unsigned short val )
	{
		return addToStream< unsigned short >( val );
	}

	Text &Text::operator<<( int val )
	{
		return addToStream< int >( val );
	}

	Text &Text::operator<<( unsigned int val )
	{
		return addToStream< unsigned int >( val );
	}

	Text &Text::operator<<( long val )
	{
		return addToStream< long >( val );
	}

	Text &Text::operator<<( unsigned long val )
	{
		return addToStream< unsigned long >( val );
	}

	Text &Text::operator<<( float val )
	{
		return addToStream< float >( val );
	}

	Text &Text::operator<<( double val )
	{
		return addToStream< double >( val );
	}

	Text &Text::operator<<( const char *right )
	{
		this->Append( std::string(right) );
		return *this;
	}

	Text &Text::operator<<( const std::string &right )
	{
		this->Append( right );
		return *this;
	}

	Text &Text::operator<<( const ustring &right )
	{
		this->Append( right );
		return *this;
	}

	Text &Text::operator<<( const achar &right )
	{
		this->Append( right );
		return *this;
	}

	Text &Text::operator<<( const astring &right )
	{
		this->Append( right );
		return *this;
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


