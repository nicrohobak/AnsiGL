#ifndef __ANSIGL_UCHAR_H__
#define __ANSIGL_UCHAR_H__

/*
 * uchar.h
 *
 * The AnsiGL UTF-8 "char"...always represents a single glyph.
 */


#include <cctype>
#include <ostream>
#include <string>


namespace AnsiGL
{
    //
    // Forward declaration of achar so we can convert both ways
    //
    class achar;


    class uchar
    {
    protected:
	std::string	m_UTF8;

    public:
	uchar();
	uchar( const achar &glyph );
	uchar( const std::string &glyph );
	uchar( unsigned char glyph );

	uchar &operator=( const achar &right );
	uchar &operator=( const std::string &right );
	uchar &operator=( const unsigned char &right );

	inline uchar &operator<<( const achar &right )
	{
	    return ((*this) = right);
	}

	inline uchar &operator<<( const std::string &right )
	{
	    return ((*this) = right);
	}

	inline uchar &operator<<( const unsigned char &right )
	{
	    return ((*this) = right);
	}

	bool operator==( const uchar &right ) const;
	bool operator!=( const uchar &right ) const;
	bool operator==( const achar &right ) const;		// Compares only the glyphs
	bool operator!=( const achar &right ) const;
	bool operator==( const std::string &right ) const;	// Compares the std::string as a single unicode glyph
	bool operator!=( const std::string &right ) const;
	bool operator==( const unsigned char &right ) const;
	bool operator!=( const unsigned char &right ) const;

	const std::string &Glyph() const;
	void Glyph( const uchar &glyph );
	void Glyph( const achar &glyph );
	void Glyph( const std::string &glyph );			// Takes the first valid UTF-8 character and assigns it as our glyph
	void Glyph( unsigned char glyph );

	bool IsSpace() const					// Returns 'true' if the glyph is whitespace
	{
	    return (m_UTF8.empty() || isspace(*m_UTF8.begin()));
	}

	void Clear();

	const std::string &Render() const;			// Simply returns the glyph
    };


    extern std::ostream &operator<<( std::ostream &left, const uchar &right );
}


#endif // __ANSIGL_UCHAR_H__


