/*
 * textwindow.cpp
 *
 */


#include "AnsiGL/textwindow.h"


namespace AnsiGL
{
    TextWindow::TextWindow():
	m_MaxLines(500),
	m_Alignment(TxtAlign_Default),
	m_WordWrap(true),
	m_NewAtBottom(true),
	ScrollToNew(true)
    {
    }

    TextWindow::TextWindow( const astring &windowTitle, const Area2D &windowSize, unsigned int maxLines, ENUM_TxtAlign alignment, bool newAtBottom, const Point3D &viewportPos, bool transparentSpaces ):
	Window(windowTitle, windowSize, viewportPos, transparentSpaces),
	m_MaxLines(maxLines),
	m_Alignment(alignment),
	m_WordWrap(true),
	m_NewAtBottom(newAtBottom),
	ScrollToNew(true)
    {
    }

    void TextWindow::Width( tSizeType width )
    {
	if ( m_WordWrap )
	{
	    std::list< Text::Ptr >::iterator CurLine;

	    for ( CurLine = m_Text.begin(); CurLine != m_Text.end(); ++CurLine )
	    {
		if ( width >= 2 )
		    (*CurLine)->Width( width - 2 );
		else
		    (*CurLine)->Width( 1 );
	    }
	}

	Window::Width( width );
    }

    unsigned int TextWindow::MaxLines() const
    {
	return m_MaxLines;
    }

    void TextWindow::MaxLines( unsigned int numLines )
    {
	m_MaxLines = numLines;

	if ( m_Text.size() > numLines )
	{
	    if ( m_NewAtBottom )
	    {
		while ( m_Text.size() > numLines )
		    m_Text.pop_front();
	    }
	    else
	    {
		while ( m_Text.size() > numLines )
		    m_Text.pop_back();
	    }
	}
    }

    bool TextWindow::WordWrap() const
    {
	return m_WordWrap;
    }

    void TextWindow::WordWrap( bool wrap )
    {
	m_WordWrap = wrap;

	// TODO: Resize everything
	std::list< Text::Ptr >::iterator CurLine;

	if ( m_WordWrap )
	{
	    // Resize to the viewport width to wrap
	    for ( CurLine = m_Text.begin(); CurLine != m_Text.end(); ++CurLine )
		(*CurLine)->Width( Window::m_Contents->Width() );
	}
	else
	{
	    // Resize to the width of the string so no word-wrapping happens
	    for ( CurLine = m_Text.begin(); CurLine != m_Text.end(); ++CurLine )
		(*CurLine)->Width( (*CurLine)->Value().length() );
	}

	resetLineSpacing();
    }

    ENUM_TxtAlign TextWindow::Align() const
    {
	return m_Alignment;
    }

    void TextWindow::Align( ENUM_TxtAlign align )
    {
	m_Alignment = align;

	std::list< Text::Ptr >::iterator CurLine;

	for ( CurLine = m_Text.begin(); CurLine != m_Text.end(); ++CurLine )
	    (*CurLine)->Align( align );
    }

    void TextWindow::AddLine( const astring &line )
    {
	if ( m_NewAtBottom )
	    AddLineAtBottom( line );
	else
	    AddLineAtTop( line );
    }

    void TextWindow::AddLineAtTop( const astring &line )
    {
	if ( line.empty() )
	    return;

	Text::Ptr NewLine;

	if ( m_WordWrap )
	    NewLine = Text::Ptr( new Text(line, Window::m_Contents->Width(), m_Alignment) );
	else
	    NewLine = Text::Ptr( new Text(line, line.length(), m_Alignment) );

	m_Text.push_front( NewLine );
	Window::m_Contents->AddContent( NewLine );

	resetLineSpacing();
    }

    void TextWindow::AddLineAtBottom( const astring &line )
    {
	if ( line.empty() )
	    return;

	Text::Ptr NewLine = Text::Ptr( new Text(line, Window::m_Contents->Width(), m_Alignment) );

	m_Text.push_back( NewLine );
	Window::m_Contents->AddContent( NewLine );

	resetLineSpacing();
    }

    void TextWindow::InsertLine( const astring &line, unsigned int atLineNum )
    {
	if ( m_NewAtBottom )
	    InsertLineFromBottom( line, atLineNum );
	else
	    InsertLineFromTop( line, atLineNum );
    }

    void TextWindow::InsertLineFromTop( const astring &line, unsigned int atLineNum )
    {
	// TODO: Write Me
	resetLineSpacing();
    }

    void TextWindow::InsertLineFromBottom( const astring &line, unsigned int atLineNum )
    {
	// TODO: Write Me
	resetLineSpacing();
    }

    void TextWindow::RemoveLine( unsigned int numLines )
    {
	if ( m_NewAtBottom )
	    RemoveLineFromTop( numLines, 0 );
	else
	    RemoveLineFromBottom( numLines, 0 );
    }

    void TextWindow::RemoveLineFromTop( unsigned int numLines, unsigned int atLineNum )
    {
	// TODO: Write Me
    }

    void TextWindow::RemoveLineFromBottom( unsigned int numLines, unsigned int atLineNum )
    {
	// TODO: Write Me
    }

    void TextWindow::resetLineSpacing()
    {
	Point2D CurPoint( 0, 0 );

	if ( 1 ) //m_NewAtBottom )
	{
	    std::list< Text::Ptr >::iterator CurLine;

	    for ( CurLine = m_Text.begin(); CurLine != m_Text.end(); ++CurLine )
	    {
		(*CurLine)->MoveTo( CurPoint );
		CurPoint.Y( CurPoint.Y() + (*CurLine)->Height() );
	    }
	}
	else
	{
	    std::list< Text::Ptr >::reverse_iterator CurLine;

	    for ( CurLine = m_Text.rbegin(); CurLine != m_Text.rend(); ++CurLine )
	    {
		(*CurLine)->MoveTo( CurPoint );
		CurPoint.Y( CurPoint.Y() + (*CurLine)->Height() );
	    }
	}

	RecalculateTotalContentSize();

	if ( ScrollToNew )
	{
	    if ( m_NewAtBottom )
	    {
		CurPoint.Y( CurPoint.Y() - Window::m_Contents->Height() );
		MoveViewportTo( CurPoint );
	    }
	    else
		MoveViewportTo( Point3D(0,0,0) );
	}
    }
}





