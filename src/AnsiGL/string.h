#ifndef __ANSIGL_STRING_H__
#define __ANSIGL_STRING_H__

/*
 * std::string.h
 *
 * A std::string-like implementation for any reasonably char-like data type
 *
 * TODO: Make this at least as fully-featured as std::string
 */


#include <cctype>
#include <cstdarg>
#include <string>
#include <sstream>
#include <vector>

#include <boost/shared_ptr.hpp>

#include "achar.h"			// For various astring needs


namespace AnsiGL
{
    //
    // Global variables
    //
    const int DEFAULT_LINE_WIDTH = 78;						// Default line width for word-wrap functions -- default terminal width is 80, 79 should be max, 78 to be safe because some terminals handle it strangely

    enum ENUM_TxtAlign
    {
	TxtAlign_First,

	TxtAlign_Left = TxtAlign_First,
	TxtAlign_Center,
	TxtAlign_Right,

	TxtAlign_Max,
	TxtAlign_Default = TxtAlign_First
    };


    template < class tCharType >
    class String
    {
    public:
	typedef boost::shared_ptr< String<tCharType> >				Ptr;

	typedef typename std::vector< tCharType >::iterator			iterator;
	typedef typename std::vector< tCharType >::reverse_iterator		reverse_iterator;
	typedef typename std::vector< tCharType >::const_iterator		const_iterator;
	typedef typename std::vector< tCharType >::const_reverse_iterator	const_reverse_iterator;

    protected:
	std::vector< tCharType >	m_String;

    public:
	String()
	{
	}

	String( int reserveSize )
	{
	    m_String.resize( reserveSize );
	}

	String( const std::string &str )
	{
	    (*this) = str;
	}

	String( const char *str )
	{
	    (*this) = std::string( str );
	}

	virtual ~String()
	{
	}

	tCharType &operator[]( size_t pos )
	{
	    return m_String[pos];
	}

	const tCharType &operator[]( size_t pos ) const
	{
	    return m_String[pos];
	}

	String< tCharType > &operator+=( const String< tCharType > &right )
	{
	    if ( right.empty() )
		return (*this);

	    typename String< tCharType >::const_iterator CurChar;

	    for ( CurChar = right.begin(); CurChar != right.end(); ++CurChar )
		push_back( *CurChar );

	    return (*this);
	}

	bool operator==( const String< tCharType > &right ) const
	{
	    if ( length() != right.length() )
		return false;

	    typename String< tCharType >::const_iterator LeftCh, RightCh;

	    for ( LeftCh = begin(), RightCh = right.begin(); LeftCh != end() && RightCh != right.end(); ++LeftCh, ++RightCh )
	    {
		if ( (*LeftCh) != (*RightCh) )
		    return false;
	    }

	    return true;
	}

	bool operator!=( const String< tCharType > &right ) const
	{
	    return !((*this) == right);
	}

	iterator begin()
	{
	    return m_String.begin();
	}

	iterator end()
	{
	    return m_String.end();
	}

	reverse_iterator rbegin()
	{
	    return m_String.rbegin();
	}

	reverse_iterator rend()
	{
	    return m_String.rend();
	}

	const_iterator begin() const
	{
	    return m_String.begin();
	}

	const_iterator end() const
	{
	    return m_String.end();
	}

	const_reverse_iterator rbegin() const
	{
	    return m_String.rbegin();
	}

	const_reverse_iterator rend() const
	{
	    return m_String.rend();
	}

	void append( const String< tCharType > &str )
	{
	    (*this) += str;
	}

	void assign( const String< tCharType > &str )
	{
	    (*this) = str;
	}

	bool compare( const String< tCharType > &right ) const		// Like std::string::compare, these return true if different
	{
	    return !((*this) == right);
	}

	void clear()
	{
	    m_String.clear();
	}

	bool empty() const
	{
	    return m_String.empty();
	}

	iterator insert( iterator pos, const tCharType &x )
	{
	    m_String.insert( pos, x );
	}

	void insert( iterator pos, size_t n, const tCharType &x )
	{
	    m_String.insert( pos, n, x );
	}

	template <class InputIterator>
	void insert( iterator pos, InputIterator first, InputIterator last )
	{
	    m_String.insert( pos, first, last );
	}

	size_t length() const
	{
	    return m_String.size();
	}

	void push_back( const tCharType &ch )
	{
	    m_String.push_back( ch );
	}

	void pop_back()
	{
	    m_String.pop_back();
	}

	//
	// Functions for std::string conversions/compatibility
	//
	// NOTE: Just about every derived class will need to have its own version of these function...any derived classes that have 'encoding' within the std::string type, for sure
	String< tCharType > &operator=( const char *right )
	{
	    clear();

	    if ( !right || right[0] == '\0' )
		return (*this);

	    (*this) = String< tCharType >( right );
	    return (*this);
	}

	String< tCharType > &operator=( const std::string &right )
	{
	    clear();

	    if ( right.empty() )
		return (*this);

	    std::string::const_iterator CurChar;

	    for ( CurChar = right.begin(); CurChar != right.end(); ++CurChar )
		push_back( tCharType(*CurChar) );

	    return (*this);
	}

	String< tCharType > &operator+=( const char *right )
	{
	    if ( !right || right[0] == '\0' )
		return (*this);

	    (*this) += String< tCharType >( right );
	    return (*this);
	}

	String< tCharType > &operator+=( const std::string &right )
	{
	    if ( right.empty() )
		return (*this);

	    (*this) += String< tCharType >( right );
	    return (*this);
	}

	// Streamify these strings
	inline String< tCharType > &operator<<( bool val )
	{
	    return addToStream< bool >( val );
	}

	inline String< tCharType > &operator<<( char val )
	{
	    return addToStream< char >( val );
	}

	inline String< tCharType > &operator<<( unsigned char val )
	{
	    return addToStream< unsigned char >( val );
	}

	inline String< tCharType > &operator<<( short val )
	{
	    return addToStream< short >( val );
	}

	inline String< tCharType > &operator<<( unsigned short val )
	{
	    return addToStream< unsigned short >( val );
	}

	inline String< tCharType > &operator<<( int val )
	{
	    return addToStream< int >( val );
	}

	inline String< tCharType > &operator<<( unsigned int val )
	{
	    return addToStream< unsigned int >( val );
	}

	inline String< tCharType > &operator<<( long val )
	{
	    return addToStream< long >( val );
	}

	inline String< tCharType > &operator<<( unsigned long val )
	{
	    return addToStream< unsigned long >( val );
	}

	inline String< tCharType > &operator<<( float val )
	{
	    return addToStream< float >( val );
	}

	inline String< tCharType > &operator<<( double val )
	{
	    return addToStream< double >( val );
	}

	inline String< tCharType > &operator<<( const char *right )
	{
	    return ((*this) += right);
	}

	inline String< tCharType > &operator<<( const std::string &right )
	{
	    return ((*this) += right);
	}

	// NOTE: Many derived classes will need to consider haing their own version of this function as well
	bool operator==( const std::string &right ) const
	{
	    if ( length() != right.length() )
		return false;

	    String< tCharType >::const_iterator CurChar;
	    std::string::const_iterator CurOther;

	    for ( CurChar = begin(), CurOther = right.begin(); CurChar != end() && CurOther != right.end(); ++CurChar, ++CurOther )
	    {
		if ( (*CurChar) != (*CurOther) )
		    return false;
	    }

	    return true;
	}

	bool operator!=( const std::string &right ) const
	{
	    return !((*this) == right);
	}

	bool compare( const std::string &right ) const
	{
	    return !((*this) == right);
	}

	void push_back( char ch )
	{
	    push_back( tCharType(ch) );
	}

	//
	// Non-std::string functions, but standard functions for String<>s
	//
	virtual std::string Render() const				// Renders to a standard std::string
	{
	    std::stringstream RenderedStr("");
	    typename String< tCharType >::const_iterator CurChar;

	    for ( CurChar = begin(); CurChar != end(); ++CurChar )
		RenderedStr << (*CurChar);

	    return RenderedStr.str();
	}

	virtual std::string str() const
	{
	    return Render();
	}

	size_t NumLines() const						// Reports the current number of lines in this string
	{
	    size_t LineCount = 0;

	    if ( m_String.empty() )
		return LineCount;

	    typename String< tCharType >::iterator CurChar;

	    for ( CurChar = begin(); CurChar != end(); ++CurChar )
	    {
		if ( !CurChar->Glyph().empty() && CurChar->Glyph().c_str()[0] == '\n' )
		    ++LineCount;
	    }

	    return LineCount;
	}

	virtual void RemoveLeadingSpace()				// Eats the whitespace at the front of the string
	{
	    for ( iterator CurChar = begin(); CurChar != end(); CurChar = begin() )
	    {
		if ( !CurChar->IsSpace() )
		    break;

		m_String.erase( CurChar );
	    } 
	}

	virtual void RemoveTrailingSpace()				// Eats the whitespace at the end of the string
	{
	    for ( reverse_iterator CurChar = rbegin(); CurChar != rend(); CurChar = rbegin() )
	    {
		if ( !CurChar->IsSpace() )
		    break;

		m_String.erase( --(CurChar.base()) );
	    }
	}

	virtual void ToUpper()						// Capitalizes everything in the string
	{
	    std::string CurGlyph;

	    for ( iterator CurChar = begin(); CurChar != end(); ++CurChar )
	    {
		CurGlyph = CurChar->Glyph();

		if ( CurGlyph.length() != 1 )
		    continue;

		CurChar->Glyph( toupper(CurGlyph.c_str()[0]) );
	    }
	}

	virtual void ToLower()						// Lowercases (is this even a word?) everything the string
	{
	    std::string CurGlyph;

	    for ( iterator CurChar = begin(); CurChar != end(); ++CurChar )
	    {
		CurGlyph = CurChar->Glyph();

		if ( CurGlyph.length() != 1 )
		    continue;

		CurChar->Glyph( tolower(CurGlyph.c_str()[0]) );
	    }
	}

	virtual void Capitalize()					// Capitalize the first character of the string (maybe expand to after periods as well?  But what about abbreviations?...)
	{
	    iterator First = begin();

	    if ( First != end() )
	    {
		std::string Char = First->Glyph();

		if ( Char.length() > 1 )
		    return;

		First->Glyph( toupper(Char.c_str()[0]) );
	    }
	}

    protected:
	template <typename tDataType>
	ustring &addToStream( tDataType val )
	{
	    std::stringstream Buffer("");
	    Buffer << val;
	    (*this) += Buffer.str();
	    return (*this);
	}
    };



    class ustring : public String< uchar >
    {
    public:
	typedef boost::shared_ptr< ustring >			Ptr;

    public:
	ustring();
	ustring( const astring &str );
	ustring( const std::string &str );

	bool compare( const astring &right ) const;

	ustring &operator=( const std::string &right );
	ustring &operator+=( const std::string &right );

	ustring &operator=( const astring &right );
	ustring &operator+=( const astring &right );

	inline ustring &operator<<( bool val )
	{
	    return addToStream< bool >( val );
	}
	
	inline ustring &operator<<( char val )
	{
	    return addToStream< char >( val );
	}
	
	inline ustring &operator<<( unsigned char val )
	{
	    return addToStream< unsigned char >( val );
	}
	
	inline ustring &operator<<( short val )
	{
	    return addToStream< short >( val );
	}
	
	inline ustring &operator<<( unsigned short val )
	{
	    return addToStream< unsigned short >( val );
	}
	
	inline ustring &operator<<( int val )
	{
	    return addToStream< int >( val );
	}
	
	inline ustring &operator<<( unsigned int val )
	{
	    return addToStream< unsigned int >( val );
	}
	
	inline ustring &operator<<( long val )
	{
	    return addToStream< long >( val );
	}
	
	inline ustring &operator<<( unsigned long val )
	{
	    return addToStream< unsigned long >( val );
	}
	
	inline ustring &operator<<( float val )
	{
	    return addToStream< float >( val );
	}
	
	inline ustring &operator<<( double val )
	{
	    return addToStream< double >( val );
	}

	inline ustring &operator<<( const char *right )
	{
	    return ((*this) += right );
	}

	inline ustring &operator<<( const astring &right )
	{
	    return ((*this) += right );
	}

	bool operator==( const astring &right ) const;
	bool operator!=( const astring &right ) const;
	bool operator==( const std::string &right ) const;

    protected:
	template <typename tDataType>
	ustring &addToStream( tDataType val )
	{
	    std::stringstream Buffer("");
	    Buffer << val;
	    (*this) += Buffer.str();
	    return (*this);
	}
    };

    std::ostream &operator<<( std::ostream &left, const ustring &right );


    class astring : public String< achar >
    {
    public:
	typedef boost::shared_ptr< astring >			Ptr;

    public:
	astring();
	astring( const ustring &str );
	astring( const std::string &str );

	bool compare( const ustring &right ) const;
	bool compare( const std::string &right ) const;

	astring &operator=( const std::string &right );
	astring &operator+=( const std::string &right );

	astring &operator=( const ustring &right );
	astring &operator+=( const ustring &right );

	inline astring &operator<<( bool val )
	{
	    return addToStream< bool >( val );
	}
	
	inline astring &operator<<( char val )
	{
	    return addToStream< char >( val );
	}
	
	inline astring &operator<<( unsigned char val )
	{
	    return addToStream< unsigned char >( val );
	}
	
	inline astring &operator<<( short val )
	{
	    return addToStream< short >( val );
	}
	
	inline astring &operator<<( unsigned short val )
	{
	    return addToStream< unsigned short >( val );
	}
	
	inline astring &operator<<( int val )
	{
	    return addToStream< int >( val );
	}
	
	inline astring &operator<<( unsigned int val )
	{
	    return addToStream< unsigned int >( val );
	}
	
	inline astring &operator<<( long val )
	{
	    return addToStream< long >( val );
	}
	
	inline astring &operator<<( unsigned long val )
	{
	    return addToStream< unsigned long >( val );
	}
	
	inline astring &operator<<( float val )
	{
	    return addToStream< float >( val );
	}
	
	inline astring &operator<<( double val )
	{
	    return addToStream< double >( val );
	}

	inline astring &operator<<( const char *right )
	{
	    return ((*this) += right );
	}

	inline astring &operator<<( const ustring &right )
	{
	    return ((*this) += right );
	}

	bool operator==( const ustring &right ) const;
	bool operator!=( const ustring &right ) const;
	bool operator==( const std::string &right ) const;

	virtual std::string Render() const;
	virtual std::string str() const;

	std::string Render( ENUM_ColorDepth desiredDepth, bool enableAnsi = true, bool enableBell = true ) const;

	virtual astring Format( unsigned int lineWidth = DEFAULT_LINE_WIDTH, ENUM_TxtAlign align = TxtAlign_Default, const ustring wrapAfter = ustring("-"), unsigned int tabSize = 8, bool newlineOnly = false ) const;	// Wraps on any whitespace or at a hyphen, and wraps to fit within lineWidth with the proper alignment formatting -- returns a new string. -- If newlineOnly = true, only the \n character will be used for line breaks

    protected:
	// Format() helper functions
	void addNewline( unsigned int lineWidth = DEFAULT_LINE_WIDTH, ENUM_TxtAlign align = TxtAlign_Default, bool newlineOnly = false );
	void adjustAlignment( unsigned int lineWidth = DEFAULT_LINE_WIDTH, ENUM_TxtAlign align = TxtAlign_Default );

	template <typename tDataType>
	astring &addToStream( tDataType val )
	{
	    std::stringstream Buffer("");
	    Buffer << val;
	    (*this) += Buffer.str();
	    return (*this);
	}
    };

    std::ostream &operator<<( std::ostream &left, const astring &right );


    ustring usprintf( const std::string &format, ... );
    astring asprintf( const std::string &format, ... );
}


#endif // __ANSIGL_STRING_H__




