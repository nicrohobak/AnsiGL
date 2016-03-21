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
#include <memory>
#include <string>
#include <sstream>
#include <vector>

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


	template <typename tCharType>
	class String
	{
	public:
		typedef String< tCharType >							tString;

		ANSIGL_POINTERS( tString )

		typedef typename std::vector< tCharType >			tVector;
		typedef typename tVector::iterator					iterator;
		typedef typename tVector::reverse_iterator			reverse_iterator;
		typedef typename tVector::const_iterator			const_iterator;
		typedef typename tVector::const_reverse_iterator	const_reverse_iterator;

	protected:
		tVector		_String;

	public:
		String()
		{
		}

		String( int reserveSize )
		{
			_String.resize( reserveSize );
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
			return _String[pos];
		}

		const tCharType &operator[]( size_t pos ) const
		{
			return _String[pos];
		}

		tString &operator+=( const tString &right )
		{
			if ( right.empty() )
				return (*this);

			const_iterator CurChar;

			for ( CurChar = right.begin(); CurChar != right.end(); ++CurChar )
				push_back( *CurChar );

			return (*this);
		}

		bool operator==( const tString &right ) const
		{
			if ( length() != right.length() )
				return false;

			const_iterator LeftCh, RightCh;

			for ( LeftCh = begin(), RightCh = right.begin(); LeftCh != end() && RightCh != right.end(); ++LeftCh, ++RightCh )
			{
				if ( (*LeftCh) != (*RightCh) )
					return false;
			}

			return true;
		}

		bool operator!=( const tString &right ) const
		{
			return !((*this) == right);
		}

		iterator begin()
		{
			return _String.begin();
		}

		iterator end()
		{
			return _String.end();
		}

		reverse_iterator rbegin()
		{
			return _String.rbegin();
		}

		reverse_iterator rend()
		{
			return _String.rend();
		}

		const_iterator begin() const
		{
			return _String.begin();
		}

		const_iterator end() const
		{
			return _String.end();
		}

		const_reverse_iterator rbegin() const
		{
			return _String.rbegin();
		}

		const_reverse_iterator rend() const
		{
			return _String.rend();
		}

		void append( const tString &str )
		{
			(*this) += str;
		}

		void assign( const tString &str )
		{
			(*this) = str;
		}

		bool compare( const tString &right ) const		// Like std::string::compare, these return true if different
		{
			return !((*this) == right);
		}

		void clear()
		{
			_String.clear();
		}

		bool empty() const
		{
			return _String.empty();
		}

		iterator insert( iterator pos, const tCharType &x )
		{
			_String.insert( pos, x );
		}

		void insert( iterator pos, size_t n, const tCharType &x )
		{
			_String.insert( pos, n, x );
		}

		template <class InputIterator>
		void insert( iterator pos, InputIterator first, InputIterator last )
		{
			_String.insert( pos, first, last );
		}

		// -1 = npos...a.k.a. "the whole string"
		tString &erase( size_t pos = 0, size_t len = -1 )
		{
			size_t p = 0;			// Position counter
			for ( auto c = _String.begin(), c_end = _String.end(); c != c_end; ++c, ++p )
			{
				if ( p >= pos )
				{
					size_t l = 0;	// Length counter
					for ( auto h = c; h != c_end; ++h, ++l )
					{
						if ( l >= len )
						{
							// Erase from the character 'c' we found first, to the character 'h' we found now
							erase( c, h );
							break;
						}
					}

					break;
				}
			}

			return (*this);
		}

		iterator erase( const_iterator pos )
		{
			return _String.erase( pos );
		}

		iterator erase( const_iterator first, const_iterator last )
		{
			return _String.erase( first, last );
		}

		size_t length() const
		{
			return _String.size();
		}

		void push_back( const tCharType &ch )
		{
			_String.push_back( ch );
		}

		void pop_back()
		{
			_String.pop_back();
		}

		//
		// Functions for std::string conversions/compatibility
		//
		// NOTE: Just about every derived class will need to have its own version of these function...any derived classes that have 'encoding' within the std::string type, for sure
		tString &operator=( const char *right )
		{
			clear();

			if ( !right || right[0] == '\0' )
				return (*this);

			(*this) = tString( right );
			return (*this);
		}

		tString &operator=( const std::string &right )
		{
			clear();

			if ( right.empty() )
				return (*this);

			std::string::const_iterator CurChar;

			for ( CurChar = right.begin(); CurChar != right.end(); ++CurChar )
				push_back( tCharType(*CurChar) );

			return (*this);
		}

		tString &operator+=( const char *right )
		{
			if ( !right || right[0] == '\0' )
				return (*this);

			(*this) += tString( right );
			return (*this);
		}

		tString &operator+=( const std::string &right )
		{
			if ( right.empty() )
				return (*this);

			(*this) += tString( right );
			return (*this);
		}

		// Streamify these strings
		tString &operator<<( bool val )
		{
			return addToStream< bool >( val );
		}

		tString &operator<<( char val )
		{
			return addToStream< char >( val );
		}

		tString &operator<<( unsigned char val )
		{
			return addToStream< unsigned char >( val );
		}

		tString &operator<<( short val )
		{
			return addToStream< short >( val );
		}

		tString &operator<<( unsigned short val )
		{
			return addToStream< unsigned short >( val );
		}

		tString &operator<<( int val )
		{
			return addToStream< int >( val );
		}

		tString &operator<<( unsigned int val )
		{
			return addToStream< unsigned int >( val );
		}

		tString &operator<<( long val )
		{
			return addToStream< long >( val );
		}

		tString &operator<<( unsigned long val )
		{
			return addToStream< unsigned long >( val );
		}

		tString &operator<<( float val )
		{
			return addToStream< float >( val );
		}

		tString &operator<<( double val )
		{
			return addToStream< double >( val );
		}

		tString &operator<<( const char *right )
		{
			return ((*this) += right);
		}

		tString &operator<<( const std::string &right )
		{
			return ((*this) += right);
		}

		// NOTE: Many derived classes will need to consider haing their own version of this function as well
		bool operator==( const std::string &right ) const
		{
			if ( length() != right.length() )
				return false;

			const_iterator CurChar;
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
			const_iterator CurChar;

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

			if ( _String.empty() )
				return LineCount;

			iterator CurChar;

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

				_String.erase( CurChar );
			}
		}

		virtual void RemoveTrailingSpace()				// Eats the whitespace at the end of the string
		{
			for ( reverse_iterator CurChar = rbegin(); CurChar != rend(); CurChar = rbegin() )
			{
				if ( !CurChar->IsSpace() )
					break;

				_String.erase( --(CurChar.base()) );
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
		ANSIGL_POINTERS( ustring )

	public:
		ustring();
		ustring( const astring &str );
		ustring( const std::string &str );

		bool compare( const astring &right ) const;

		ustring &operator=( const std::string &right );
		ustring &operator+=( const std::string &right );

		ustring &operator=( const astring &right );
		ustring &operator+=( const astring &right );

		ustring &operator<<( bool val )
		{
			return addToStream< bool >( val );
		}

		ustring &operator<<( char val )
		{
			return addToStream< char >( val );
		}

		ustring &operator<<( unsigned char val )
		{
			return addToStream< unsigned char >( val );
		}

		ustring &operator<<( short val )
		{
			return addToStream< short >( val );
		}

		ustring &operator<<( unsigned short val )
		{
			return addToStream< unsigned short >( val );
		}

		ustring &operator<<( int val )
		{
			return addToStream< int >( val );
		}

		ustring &operator<<( unsigned int val )
		{
			return addToStream< unsigned int >( val );
		}

		ustring &operator<<( long val )
		{
			return addToStream< long >( val );
		}

		ustring &operator<<( unsigned long val )
		{
			return addToStream< unsigned long >( val );
		}

		ustring &operator<<( float val )
		{
			return addToStream< float >( val );
		}

		ustring &operator<<( double val )
		{
			return addToStream< double >( val );
		}

		ustring &operator<<( const char *right )
		{
			return ((*this) += right );
		}

		ustring &operator<<( const astring &right )
		{
			return ((*this) += right );
		}

		bool operator==( const astring &right ) const;
		bool operator!=( const astring &right ) const;
		bool operator==( const ustring &right ) const;
		bool operator!=( const ustring &right ) const;
		bool operator==( const std::string &right ) const;
		bool operator!=( const std::string &right ) const;

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
		ANSIGL_POINTERS( astring )

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

		astring &operator+=( const uchar &right );
		astring &operator+=( const achar &right );
		astring &operator+=( const astring &right );

		astring &operator<<( void *val )
		{
			return addToStream< void * >( val );
		}

		astring &operator<<( bool val )
		{
			return addToStream< bool >( val );
		}

		astring &operator<<( char val )
		{
			return addToStream< char >( val );
		}

		astring &operator<<( unsigned char val )
		{
			return addToStream< unsigned char >( val );
		}

		astring &operator<<( short val )
		{
			return addToStream< short >( val );
		}

		astring &operator<<( unsigned short val )
		{
			return addToStream< unsigned short >( val );
		}

		astring &operator<<( int val )
		{
			return addToStream< int >( val );
		}

		astring &operator<<( unsigned int val )
		{
			return addToStream< unsigned int >( val );
		}

		astring &operator<<( long val )
		{
			return addToStream< long >( val );
		}

		astring &operator<<( unsigned long val )
		{
			return addToStream< unsigned long >( val );
		}

		astring &operator<<( float val )
		{
			return addToStream< float >( val );
		}

		astring &operator<<( double val )
		{
			return addToStream< double >( val );
		}

		astring &operator<<( const char *right )
		{
			return ((*this) += right);
		}

		astring &operator<<( const uchar &right )
		{
			return ((*this) += right);
		}

		astring &operator<<( const achar &right )
		{
			return ((*this) += right);
		}

		astring &operator<<( const ustring &right )
		{
			return ((*this) += right);
		}

		astring &operator<<( const astring &right )
		{
			return ((*this) += right);
		}

		bool operator==( const astring &right ) const;
		bool operator!=( const astring &right ) const;
		bool operator==( const ustring &right ) const;
		bool operator!=( const ustring &right ) const;
		bool operator==( const std::string &right ) const;
		bool operator!=( const std::string &right ) const;

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


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --indent-switches --pad-oper


