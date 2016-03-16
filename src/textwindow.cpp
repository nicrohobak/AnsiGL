/*
 * textwindow.cpp
 *
 */


#include "AnsiGL/textwindow.h"


namespace AnsiGL
{
	TextWindow::TextWindow():
		_MaxLines(500),
		_Alignment(TxtAlign_Default),
		_WordWrap(true),
		_NewAtBottom(true),
		ScrollToNew(true)
	{
	}

	TextWindow::TextWindow( const astring &windowTitle, const Area2D &windowSize, unsigned int maxLines, ENUM_TxtAlign alignment, bool newAtBottom, const Point3D &viewportPos, bool transparentSpaces ):
		Window(windowTitle, windowSize, viewportPos, transparentSpaces),
		_MaxLines(maxLines),
		_Alignment(alignment),
		_WordWrap(true),
		_NewAtBottom(newAtBottom),
		ScrollToNew(true)
	{
	}

	void TextWindow::Width( tSizeType width )
	{
		if ( _WordWrap )
		{
			std::list< Text::Ptr >::iterator CurLine;

			for ( CurLine = _Text.begin(); CurLine != _Text.end(); ++CurLine )
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
		return _MaxLines;
	}

	void TextWindow::MaxLines( unsigned int numLines )
	{
		_MaxLines = numLines;

		if ( _Text.size() > numLines )
		{
			if ( _NewAtBottom )
			{
				while ( _Text.size() > numLines )
					_Text.pop_front();
			}
			else
			{
				while ( _Text.size() > numLines )
					_Text.pop_back();
			}
		}
	}

	bool TextWindow::WordWrap() const
	{
		return _WordWrap;
	}

	void TextWindow::WordWrap( bool wrap )
	{
		_WordWrap = wrap;

		// TODO: Resize everything
		std::list< Text::Ptr >::iterator CurLine;

		if ( _WordWrap )
		{
			// Resize to the viewport width to wrap
			for ( CurLine = _Text.begin(); CurLine != _Text.end(); ++CurLine )
				(*CurLine)->Width( Window::_Contents->Width() );
		}
		else
		{
			// Resize to the width of the string so no word-wrapping happens
			for ( CurLine = _Text.begin(); CurLine != _Text.end(); ++CurLine )
				(*CurLine)->Width( (*CurLine)->Value().length() );
		}

		resetLineSpacing();
	}

	ENUM_TxtAlign TextWindow::Align() const
	{
		return _Alignment;
	}

	void TextWindow::Align( ENUM_TxtAlign align )
	{
		_Alignment = align;

		std::list< Text::Ptr >::iterator CurLine;

		for ( CurLine = _Text.begin(); CurLine != _Text.end(); ++CurLine )
			(*CurLine)->Align( align );
	}

	void TextWindow::AddLine( const astring &line )
	{
		if ( _NewAtBottom )
			AddLineAtBottom( line );
		else
			AddLineAtTop( line );
	}

	void TextWindow::AddLineAtTop( const astring &line )
	{
		if ( line.empty() )
			return;

		Text::Ptr NewLine;

		if ( _WordWrap )
			NewLine = Text::Ptr( new Text(line, Window::_Contents->Width(), _Alignment) );
		else
			NewLine = Text::Ptr( new Text(line, line.length(), _Alignment) );

		_Text.push_front( NewLine );
		Window::_Contents->AddContent( NewLine );

		resetLineSpacing();
	}

	void TextWindow::AddLineAtBottom( const astring &line )
	{
		if ( line.empty() )
			return;

		Text::Ptr NewLine = Text::Ptr( new Text(line, Window::_Contents->Width(), _Alignment) );

		_Text.push_back( NewLine );
		Window::_Contents->AddContent( NewLine );

		resetLineSpacing();
	}

	void TextWindow::InsertLine( const astring &line, unsigned int atLineNum )
	{
		if ( _NewAtBottom )
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
		if ( _NewAtBottom )
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

			for ( CurLine = _Text.begin(); CurLine != _Text.end(); ++CurLine )
			{
				(*CurLine)->MoveTo( CurPoint );
				CurPoint.Y( CurPoint.Y() + (*CurLine)->Height() );
			}
		}
		else
		{
			std::list< Text::Ptr >::reverse_iterator CurLine;

			for ( CurLine = _Text.rbegin(); CurLine != _Text.rend(); ++CurLine )
			{
				(*CurLine)->MoveTo( CurPoint );
				CurPoint.Y( CurPoint.Y() + (*CurLine)->Height() );
			}
		}

		RecalculateTotalContentSize();

		if ( ScrollToNew )
		{
			if ( _NewAtBottom )
			{
				CurPoint.Y( CurPoint.Y() - Window::_Contents->Height() );
				MoveViewportTo( CurPoint );
			}
			else
				MoveViewportTo( Point3D(0, 0, 0) );
		}
	}
}


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --pad-oper


