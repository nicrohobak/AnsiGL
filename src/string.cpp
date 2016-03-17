/*
 * std::string.cpp
 *
 * A std::string-like implementation for any reasonably char-like data type
 */


#include <cstdio>

#include "AnsiGL/achar.h"
#include "AnsiGL/string.h"


namespace AnsiGL
{
	ustring::ustring()
	{
	}

	ustring::ustring( const astring &str )
	{
		(*this) = str;
	}

	ustring::ustring( const std::string &str )
	{
		(*this) = str;
	}

	bool ustring::compare( const astring &right ) const
	{
		return !(*this == right);
	}

	ustring &ustring::operator=( const std::string &right )
	{
		clear();

		if ( right.empty() )
			return (*this);

		std::string::const_iterator CurChar;

		for ( CurChar = right.begin(); CurChar != right.end(); ++CurChar )
		{
			if ( (*CurChar) > 0 && (*CurChar) < 128 )
			{
				// Standard ASCII(-like) character
				push_back( *CurChar );
				continue;
			}
			else
			{
				// Unicoded!  Find out how many bytes...
				char Check = (*CurChar);
				std::string CurGlyph("");
				int TotalBytes = 0;
				int NumBytes = 0;

				// Figure out the number of bytes we're supposed to have
				for ( ; Check & (1 << 8); Check <<= 1 )
					++TotalBytes;

				// If it's less than or just 1, then it's malformed unicode
				if ( TotalBytes <= 1 )
					continue;

				std::string::const_iterator CurByte;

				for ( CurByte = CurChar; CurByte != right.end(); ++CurByte )
				{
					// If the current byte isn't possibly unicode, then we're done here
					if ( !((*CurByte) & (1 << 8)) )
						break;

					CurGlyph.push_back( *CurByte );

					if ( ++NumBytes >= TotalBytes )
						break;
				}

				push_back( uchar(CurGlyph) );

				// Update our other iterator to the real new position
				CurChar = CurByte;
			}
		}

		return (*this);
	}

	ustring &ustring::operator+=( const std::string &right )
	{
		if ( right.empty() )
			return (*this);

		append( ustring(right) );
		return (*this);
	}

	ustring &ustring::operator=( const astring &right )
	{
		clear();

		if ( right.empty() )
			return (*this);

		astring::const_iterator CurAChar;

		for ( CurAChar = right.begin(); CurAChar != right.end(); ++CurAChar )
			push_back( CurAChar->Glyph() );

		return (*this);
	}

	ustring &ustring::operator+=( const astring &right )
	{
		if ( right.empty() )
			return (*this);

		append( ustring(right) );
		return (*this);
	}

	bool ustring::operator==( const astring &right ) const
	{
		ustring Temp( right );
		return ((*this) == Temp);
	}

	bool ustring::operator!=( const astring &right ) const
	{
		return !((*this) == right);
	}

	bool ustring::operator==( const ustring &right ) const
	{
		if ( length() != right.length() )
			return false;

		for ( auto LeftCh = begin(),
		        LeftCh_End = end(),
		        RightCh = right.begin(),
		        RightCh_End = right.end();
		        LeftCh != LeftCh_End && RightCh != RightCh_End;
		        ++LeftCh, ++RightCh )
		{
			if ( *LeftCh != *RightCh )
				return false;
		}

		return true;
	}

	bool ustring::operator!=( const ustring &right ) const
	{
		return !((*this) == right);
	}

	bool ustring::operator==( const std::string &right ) const
	{
		ustring Temp( right );
		return ((*this) == Temp);
	}

	bool ustring::operator!=( const std::string &right ) const
	{
		return !((*this) == right);
	}


	std::ostream &operator<<( std::ostream &left, const ustring &right )
	{
		left << right.Render();
		return left;
	}


	astring::astring()
	{
	}

	astring::astring( const ustring &str )
	{
		(*this) = str;
	}

	astring::astring( const std::string &str )
	{
		(*this) = str;
	}

	bool astring::compare( const std::string &right ) const
	{
		return !(*this == right);
	}

	astring &astring::operator=( const std::string &right )
	{
		(*this) = ConvertColorCodes( ustring(right) );
		return (*this);
	}

	astring &astring::operator+=( const std::string &right )
	{
		if ( right.empty() )
			return (*this);

		achar PrevANSIState;

		if ( rbegin() != rend() )
			PrevANSIState = (*rbegin());

		append( ConvertColorCodes( ustring(right), PrevANSIState ) );
		return (*this);
	}

	astring &astring::operator=( const ustring &right )
	{
		if ( right.empty() )
			return (*this);

		(*this) = ConvertColorCodes( right );
		return (*this);
	}

	astring &astring::operator+=( const ustring &right )
	{
		if ( right.empty() )
			return (*this);

		achar PrevANSIState;

		if ( rbegin() != rend() )
			PrevANSIState = (*rbegin());

		append( ConvertColorCodes( right, PrevANSIState ) );
		return (*this);
	}

	astring &astring::operator+=( const astring &right )
	{
		if ( right.empty() )
			return (*this);

		for ( auto c = right.begin(), c_end = right.end(); c != c_end; ++c )
			push_back( *c );

		return (*this);
	}

	bool astring::operator==( const astring &right ) const
	{
		if ( length() != right.length() )
			return false;

		for ( auto LeftCh = begin(),
		        LeftCh_End = end(),
		        RightCh = right.begin(),
		        RightCh_End = right.end();
		        LeftCh != LeftCh_End && RightCh != RightCh_End;
		        ++LeftCh, ++RightCh )
		{
			if ( *LeftCh != *RightCh )
				return false;
		}

		return true;
	}

	bool astring::operator!=( const astring &right ) const
	{
		return !((*this) == right);
	}

	bool astring::operator==( const ustring &right ) const
	{
		astring Temp( right );
		return ((*this) == Temp);
	}

	bool astring::operator!=( const ustring &right ) const
	{
		return !((*this) == right);
	}

	bool astring::operator==( const std::string &right ) const
	{
		if ( length() != right.length() )
			return false;

		auto LeftCh = begin(),
		     LeftCh_End = end();
		auto RightCh = right.begin(),
		     RightCh_End = right.end();

		for ( ; LeftCh != LeftCh_End && RightCh != RightCh_End; ++LeftCh, ++RightCh )
		{
			if ( *LeftCh != *RightCh )
				return false;
		}

		return true;
	}

	bool astring::operator!=( const std::string &right ) const
	{
		return !((*this) == right);
	}

	std::string astring::Render() const
	{
		return Render( ColorDepth_Default );	// Make sure it calls the other version of Render(), with ANSI, etc. by default
	}

	std::string astring::str() const
	{
		return Render( ColorDepth_6Bit, false, false );	// Render no extras, only the glyphs by default this way
	}

	std::string astring::Render( ENUM_ColorDepth desiredDepth, bool enableAnsi, bool enableBell ) const
	{
		std::stringstream RenderedStr("");
		const_iterator CurChar;
		achar CurState;				// A cache of the current state of color/attributes

		for ( CurChar = begin(); CurChar != end(); ++CurChar )
		{
			if ( (*CurChar) == '\0' )
				break;

			if ( CurChar->Bell && enableBell )
				RenderedStr << '\007';

			// If the color or attributes are different, cache them and render the new state
			if ( enableAnsi && (CurState.Color != CurChar->Color || CurState.Attributes() != CurChar->Attributes()) )
			{
				CurState = (*CurChar);
				RenderedStr << CurChar->RenderAnsi( desiredDepth );
			}

			RenderedStr << CurChar->Glyph();
		}

		// Make sure we always close the std::string with a color terminator
		if ( enableAnsi )
			RenderedStr << ANSI_CODE_START << ANSI_Default << ANSI_CODE_END;	// ANSI Default

		return RenderedStr.str();
	}

	astring astring::Format( unsigned int lineWidth, ENUM_TxtAlign align, const ustring wrapAfter, unsigned int tabSize, bool newlineOnly ) const
	{
		const_iterator CurChar;
		astring FormattedStr, CurLine, CurWord;
		unsigned int CurLineLength = 0;
		char CheckChar = 0;

		CurChar = begin();

		while ( CurChar != end() )
		{
			// If the glyph is empty, treat it as a space character
			if ( CurChar->Glyph().empty() )
				CheckChar = ' ';
			else
				CheckChar = CurChar->Glyph().c_str()[0];	// Take the first character, since spaces we care about are ASCII and > 128

			// Check for whitespace
			if ( isspace(CheckChar) )
			{
				bool SkipSpaceChar = false;		// If we add a newline before we get to our space character, we actually don't need the space character

				// Check to see if the current word will fit on the line, if not do the newline process then add it
				if ( CurLineLength + CurWord.length() > lineWidth )
				{
					CurLine.addNewline( lineWidth, align, newlineOnly );
					FormattedStr.append( CurLine );
					CurLine.clear();

					CurLineLength = 0;
					SkipSpaceChar = true;		// Since we've effectively already added a space, theres no need to add the actual character below
				}

				CurLine.append( CurWord );
				CurWord.clear();

				// Also make sure, no matter what we do, we move our iterator past this space character in the main string
				++CurChar;

				if ( SkipSpaceChar )
					continue;

				switch ( CheckChar )
				{
					case ' ':
					{
						// Check to see if a space will fit on the line too, if not do the newline process, otherwise add the space
						if ( CurLineLength + 1 > lineWidth )
						{
							CurLine.addNewline( lineWidth, align, newlineOnly );
							FormattedStr.append( CurLine );
							CurLine.clear();
							CurLineLength = 0;
						}
						else
						{
							CurLine.append( " " );
							++CurLineLength;
						}

						break;
					}

					case '\t':
					{
						// Fill up a temp string with tabSize spaces
						std::string TabStr( tabSize, ' ' );

						// Check to see if the tab will fit, if not do the newline process, otherwise add the tab
						if ( CurLineLength + tabSize > lineWidth )
						{
							CurLine.addNewline( lineWidth, align, newlineOnly );
							FormattedStr.append( CurLine );
							CurLine.clear();
							CurLineLength = 0;
						}
						else
						{
							CurLine.append( TabStr );
							CurLineLength += tabSize;
						}

						break;
					}

					case '\n':
					{
						// Check to see if the next character is '\r', if so, skip over it
						const_iterator NextChar = CurChar;

						if ( NextChar == end() )
							break;

						if ( ++NextChar != end() && NextChar->Glyph().c_str()[0] == '\r' )
							++CurChar;

						CurLine.addNewline( lineWidth, align, newlineOnly );
						FormattedStr.append( CurLine );
						CurLine.clear();
						CurLineLength = 0;
						break;
					}

					case '\r':
					{
						// Check to see if the next character is '\n', if so, skip over it
						const_iterator NextChar = CurChar;

						if ( NextChar == end() )
							break;

						if ( ++NextChar != end() && NextChar->Glyph().c_str()[0] == '\n' )
							++CurChar;

						CurLine.addNewline( lineWidth, align, newlineOnly );
						FormattedStr.append( CurLine );
						CurLine.clear();
						CurLineLength = 0;
						break;
					}

					default:
						break;
				}
			}
			else
			{
				astring::const_iterator CurWordChar;
				bool FoundWrapChar = false;

				// Reset CheckChar here, so we can reuse it for our word
				CheckChar = 0;

				// Grab a word
				for ( CurWordChar = CurChar; CurWordChar != end(); ++CurWordChar )
				{
					if ( CurWordChar->Glyph().empty() )
						CheckChar = ' ';
					else
						CheckChar = CurWordChar->Glyph().c_str()[0];

					if ( !isspace(CheckChar) )
					{
						ustring::const_iterator WrapChar;

						// Check to see if this character is one of the ones we're allowed to wrap on
						for ( WrapChar = wrapAfter.begin(); WrapChar != wrapAfter.end(); ++WrapChar )
						{
							// If the character is a wrap character, then treat this as the end of the current word
							if ( *CurWordChar == *WrapChar )
							{
								CurWord.push_back( *CurWordChar );	// Make sure to include the wrap character itself in this word!  It's a little different than normal because of this
								FoundWrapChar = true;
								break;
							}
						}

						if ( FoundWrapChar )
							break;
					}
					else
						break;

					CurWord.push_back( *CurWordChar );
				}

				// Update our main iterator
				if ( FoundWrapChar )
					CurChar = CurWordChar + 1;
				else
					CurChar = CurWordChar;

				if ( CurLineLength + CurWord.length() > lineWidth )
				{
					CurLine.addNewline( lineWidth, align, newlineOnly );
					FormattedStr.append( CurLine );
					CurLine.clear();
					CurLineLength = 0;
				}

				// If the word itself is longer than our line, then we've already wrapped thanks to the chunk above, so now...
				if ( CurWord.length() > lineWidth )
				{
					astring TempWord;
					unsigned int NumChars = 0;

					for ( iterator TempChar = CurWord.begin(); TempChar != CurWord.end(); ++TempChar, ++NumChars )
					{
						if ( NumChars >= lineWidth )
						{
							TempWord.addNewline( lineWidth, align, newlineOnly );
							FormattedStr.append( TempWord );
							TempWord.clear();
							NumChars = 0;
						}

						TempWord.push_back( *TempChar );
					}

					CurWord = TempWord;
					CurLineLength = NumChars;
				}
				else
					CurLineLength += CurWord.length();

				CurLine.append( CurWord );
				CurWord.clear();
			}
		}

		// If we get out of the main loop, make sure we get the last bit of our string
		if ( !CurWord.empty() )
		{
			if ( CurLineLength + CurWord.length() > lineWidth )
			{
				CurLine.addNewline( lineWidth, align, newlineOnly );
				FormattedStr.append( CurLine );
				CurLine.clear();
			}

			CurLine.append( CurWord );
		}

		if ( !CurLine.empty() )
		{
			CurLine.adjustAlignment( lineWidth, align );
			FormattedStr.append( CurLine );
		}

		return FormattedStr;
	}

	void astring::addNewline( unsigned int lineWidth, ENUM_TxtAlign align, bool newlineOnly )
	{
		adjustAlignment( lineWidth, align );

		if ( newlineOnly )
			append( "\n" );
		else
			append( "\r\n" );
	}

	void astring::adjustAlignment( unsigned int lineWidth, ENUM_TxtAlign align )
	{
		astring PaddedSpace;
		unsigned int ExtraSpace = 0;

		RemoveTrailingSpace();

		if ( lineWidth > length() )
			ExtraSpace = lineWidth - length();

		switch ( align )
		{
		default:
		case TxtAlign_Left:
			// Things normally fall into this alignment...nothing to do here
			break;

		case TxtAlign_Center:
			{
				// Take half of the remainder of the line width
				for ( unsigned int i = 0; i < (ExtraSpace >> 1); ++i ) // shift instead of divide by 2
					PaddedSpace.append( " " );

				break;
			}

		case TxtAlign_Right:
			{
				// Take the remainder of the line width
				for ( unsigned int i = 0; i < ExtraSpace; ++i )
					PaddedSpace.append( " " );

				break;
			}
		}

		// Now that we have the appropriate amount of padded space, prepend it
		if ( !PaddedSpace.empty() )
		{
			PaddedSpace.append( *this );		// Add this to the end of our padding
			assign( PaddedSpace );			// Then assign it to us
		}
	}


	std::ostream &operator<<( std::ostream &left, const astring &right )
	{
		left << right.Render();
		return left;
	}


	ustring usprintf( const std::string &format ... )
	{
		if ( format.empty() )
			return astring();

		const int LENGTH = format.length() * 2;
		char *buf = new char[ LENGTH ];

		va_list args;
		va_start( args, format );
		vsprintf( buf, format.c_str(), args );
		va_end( args );

		delete [] buf;
		return ustring( buf );
	}

	astring asprintf( const std::string &format ... )
	{
		if ( format.empty() )
			return astring();

		const int LENGTH = format.length() * 2;
		char *buf = new char[ LENGTH ];

		va_list args;
		va_start( args, format );
		vsprintf( buf, format.c_str(), args );
		va_end( args );

		delete [] buf;
		return ConvertColorCodes( ustring(buf) );
	}
}


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --pad-oper


