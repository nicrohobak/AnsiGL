/*
 * uchar.cpp
 *
 * The AnsiGL UTF-8 "char"...always represents a single glyph
 */


#include "AnsiGL/achar.h"
#include "AnsiGL/uchar.h"


namespace AnsiGL
{
	uchar::uchar( const achar &glyph )
	{
		Glyph( glyph.Glyph() );
	}

	uchar &uchar::operator=( const achar &right )
	{
		_UTF8 = right.Glyph();
		return *this;
	}

	uchar &uchar::operator=( const std::string &right )
	{
		Clear();

		if ( right.empty() )
			return (*this);

		// If it's less than 128, it's just ASCII, so just assign it
		if ( !(right.c_str()[0] & (1 << 8)) )
		{
			_UTF8.clear();
			_UTF8.push_back( right.c_str()[0] );
			return (*this);
		}

		char Check = right.c_str()[0];
		std::string::const_iterator CurByte;
		unsigned int TotalBytes = 0;
		unsigned int NumBytes = 0;

		// Figure out the number of bytes we're supposed to have
		for ( ; Check & (1 << 8); Check <<= 1 )
			++TotalBytes;

		_UTF8.clear();

		for ( CurByte = right.begin(); CurByte != right.end(); ++CurByte )
		{
			// If the current byte isn't possibly unicode, then we're done here
			if ( !((*CurByte) & (1 << 8)) )
				break;

			_UTF8.push_back( *CurByte );

			if ( ++NumBytes >= TotalBytes )
				break;
		}

		// If the calculated number of bytes doesn't add up to what we recovered...consider it improperly formed as well
		if ( TotalBytes != _UTF8.length() )
		{
			_UTF8.clear();
			return (*this);
		}

		// And finally, check and make sure that we aren't over-encoded with too many bytes
		Check = _UTF8.c_str()[1];
		if ( Check & (1 << (8 - TotalBytes)) )
		{
			_UTF8.clear();
			return (*this);
		}

		return (*this);
	}

	uchar &uchar::operator=( const unsigned char &right )
	{
		Glyph( right );
		return (*this);
	}

	bool uchar::operator==( const uchar &right ) const
	{
		return (!_UTF8.compare( right._UTF8 ));
	}

	bool uchar::operator!=( const uchar &right ) const
	{
		return !((*this) == right);
	}

	bool uchar::operator==( const achar &right ) const
	{
		return (!_UTF8.compare( right.Glyph() ));
	}

	bool uchar::operator!=( const achar &right ) const
	{
		return !((*this) == right.Glyph());
	}

	bool uchar::operator==( const std::string &right ) const
	{
		return (!_UTF8.compare( right ));
	}

	bool uchar::operator!=( const std::string &right ) const
	{
		return !((*this) == right);
	}

	bool uchar::operator==( const unsigned char &right ) const
	{
		if ( _UTF8.length() != 1 )
			return false;

		return (_UTF8.c_str()[0] == right);
	}

	bool uchar::operator!=( const unsigned char &right ) const
	{
		return !((*this) == right);
	}

	void uchar::Glyph( const uchar &glyph )
	{
		(*this) = glyph;
	}

	void uchar::Glyph( const achar &glyph )
	{
		(*this) = glyph;
	}

	void uchar::Glyph( const std::string &glyph )
	{
		(*this) = glyph;
	}

	void uchar::Glyph( unsigned char glyph )
	{
		if ( glyph < 128 )
		{
			_UTF8.clear();
			_UTF8.push_back( glyph );
		}
	}


	std::ostream &operator<<( std::ostream &left, const uchar &right )
	{
		left << right.Glyph();
		return left;
	}
}


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --indent-switches --pad-oper


