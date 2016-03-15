#ifndef __ANSIGL_TEXTWINDOW_H__
#define __ANSIGL_TEXTWINDOW_H__

/*
 * textwindow.h
 *
 * A Window that stores text on a line-by-line basis, 
 */


#include <list>

#include "text.h"
#include "window.h"


namespace AnsiGL
{
    const int DEFAULT_MAX_LINES = 1000;

    //
    // TextWindows take us from simply drawing on a bordered surface to actually placing text into a nice, word-wrapping (etc.) container.  This (and its entire lineage) assume to deal with 
    //
    class TextWindow : public Window
    {
    public:
	ANSIGL_POINTERS( TextWindow )

    protected:
	std::list< Text::Ptr >	m_Text;				// Lines of text, akin to messages in a chat window

	unsigned int		m_MaxLines;
	ENUM_TxtAlign		m_Alignment;
	bool			m_WordWrap;			// Word wrap to keep within the window
	bool			m_NewAtBottom;			// If true, new lines are added at the bottom by default

    public:
	bool			ScrollToNew;

    public:
	TextWindow();
	TextWindow( const astring &windowTitle, const Area2D &windowSize = Area2D(80,24), unsigned int maxLines = DEFAULT_MAX_LINES, ENUM_TxtAlign alignment = TxtAlign_Default, bool newAtBottom = true, const Point3D &viewportPos = Point3D(), bool transparentSpaces = false );

	virtual void Width( tSizeType width );

	unsigned int MaxLines() const;
	void MaxLines( unsigned int numLines );

	bool WordWrap() const;
	void WordWrap( bool wrap );

	ENUM_TxtAlign Align() const;
	void Align( ENUM_TxtAlign align );

	virtual void AddLine( const astring &line );
	virtual void AddLineAtTop( const astring &line );
	virtual void AddLineAtBottom( const astring &line );

	virtual void InsertLine( const astring &line, unsigned int atLineNum = 0 );
	virtual void InsertLineFromTop( const astring &line, unsigned int atLineNum = 0 );
	virtual void InsertLineFromBottom( const astring &line, unsigned int atLineNum = 0 );

	virtual void RemoveLine( unsigned int numLines = 1 );
	virtual void RemoveLineFromTop( unsigned int numLines = 1, unsigned int atLineNum = 0 );
	virtual void RemoveLineFromBottom( unsigned int numLines = 1, unsigned int atLineNum = 0 );

    protected:
	virtual void resetLineSpacing();			// Resets the spacing between lines
    };
}


#endif // __ANSIGL_TEXTWINDOW_H__


