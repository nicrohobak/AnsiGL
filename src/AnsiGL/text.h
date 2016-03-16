#ifndef __ANSIGL_TEXT_H__
#define __ANSIGL_TEXT_H__

/*
 * text.h
 *
 * A content object that represents a text field.
 */


#include "content.h"


namespace AnsiGL
{
	class Text : public Content
	{
	public:
		ANSIGL_POINTERS( Text )

	protected:
		astring			_Text;
		astring			_FormattedText;

		ENUM_TxtAlign		_Alignment;

	public:
		bool			AutoHeight;			// Automatically adjusts the height based on the formatted text
		bool			TransparentSpaces;

	public:
		Text();
		Text( const astring &text, tSizeType width = 0, ENUM_TxtAlign alignment = TxtAlign_Default, bool transparentSpaces = false, bool autoHeight = true );	// A width of 0 is unlimited
		~Text();

		const astring &Value() const;
		const astring &FormattedValue() const;
		void Value( const astring &text );
		void Value( const ustring &text );
		void Value( const std::string &text );

		void Append( const astring &text );
		void Append( const ustring &text );
		void Append( const std::string &text );

		void Clear();

		ENUM_TxtAlign Align() const;
		void Align( ENUM_TxtAlign align );

		virtual const tSizeType &Width() const;
		virtual void Width( tSizeType width );
		virtual const tSizeType &Height() const;
		virtual void Height( tSizeType height );
		virtual void Resize( const Area2D &size );

		virtual std::string str();
		virtual std::string Render() const;
		virtual void RenderToSurface( Surface::Ptr dest, const Point2D &pos = Point2D() ) const;

	protected:
		void format();
	};
}


#endif // __ANSIGL_TEXT_H__


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --pad-oper


