/*
 * uchar.cpp
 *
 * The AnsiGL UTF-8 "char"...always represents a single glyph
 */


#include "AnsiGL/achar.h"
#include "AnsiGL/uchar.h"


namespace AnsiGL
{
    uchar::uchar():
	m_UTF8(" ")
    {
    }

    uchar::uchar( const achar &glyph )
    {
	Glyph( glyph.Glyph() );
    }

    uchar::uchar( const std::string &glyph )
    {
	Glyph( glyph );
    }

    uchar::uchar( unsigned char glyph )
    {
	if ( glyph < 128 )
	    (*this) = glyph;
    }

    uchar &uchar::operator=( const achar &right )
    {
	m_UTF8 = right.Glyph();
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
	    m_UTF8.clear();
	    m_UTF8.push_back( right.c_str()[0] );
	    return (*this);
	}

	char Check = right.c_str()[0];
	std::string::const_iterator CurByte;
	unsigned int TotalBytes = 0;
	unsigned int NumBytes = 0;

	// Figure out the number of bytes we're supposed to have
	for ( ; Check & (1 << 8); Check <<= 1 )
	    ++TotalBytes;

	m_UTF8.clear();

	for ( CurByte = right.begin(); CurByte != right.end(); ++CurByte )
	{
	    // If the current byte isn't possibly unicode, then we're done here
	    if ( !(*CurByte) & (1 << 8))
		break;

	    m_UTF8.push_back( *CurByte );

	    if ( ++NumBytes >= TotalBytes )
		break;
	}

	// If the calculated number of bytes doesn't add up to what we recovered...consider it improperly formed as well
	if ( TotalBytes != m_UTF8.length() )
	{
	    m_UTF8.clear();
	    return (*this);
	}

	// And finally, check and make sure that we aren't over-encoded with too many bytes
	Check = m_UTF8.c_str()[1];
	if ( Check & (1 << (8 - TotalBytes)) )
	{
	    m_UTF8.clear();
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
	return (!m_UTF8.compare( right.m_UTF8 ));
    }

    bool uchar::operator!=( const uchar &right ) const
    {
	return !((*this) == right);
    }

    bool uchar::operator==( const achar &right ) const
    {
	return (!m_UTF8.compare( right.Glyph() ));
    }

    bool uchar::operator!=( const achar &right ) const
    {
	return !((*this) == right.Glyph());
    }

    bool uchar::operator==( const std::string &right ) const
    {
	return (!m_UTF8.compare( right ));
    }

    bool uchar::operator!=( const std::string &right ) const
    {
	return !((*this) == right);
    }

    bool uchar::operator==( const unsigned char &right ) const
    {
	if ( m_UTF8.length() != 1 )
	    return false;

	return (m_UTF8.c_str()[0] == right);
    }

    bool uchar::operator!=( const unsigned char &right ) const
    {
	return !((*this) == right);
    }

    const std::string &uchar::Glyph() const
    {
	return m_UTF8;
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
	    m_UTF8.clear();
	    m_UTF8.push_back( glyph );
	}
    }

    void uchar::Clear()
    {
	m_UTF8 = " ";
    }

    const std::string &uchar::Render() const
    {
	return Glyph();
    }


    std::ostream &operator<<( std::ostream &left, const uchar &right )
    {
	left << right.Glyph();
	return left;
    }
}




