/*
 * textwindow.cpp
 *
 */


#include "AnsiGL/textwindow.h"


namespace AnsiGL
{
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

	void TextWindow::Clear()
	{
		_Text.clear();
		Window::Clear();
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
		Text::Ptr NewLine;

		if ( _WordWrap )
			NewLine = std::make_shared< Text >( line, Window::_Contents->Width(), _Alignment, ColorDepth_Default );
		else
			NewLine = std::make_shared< Text >( line, line.length(), _Alignment, ColorDepth_Default );

		_Text.push_front( NewLine );
		Window::_Contents->AddContent( NewLine );

		resetLineSpacing();

		if ( ScrollToNew )
			scrollToLine( NewLine );
	}

	void TextWindow::AddLineAtBottom( const astring &line )
	{
		Text::Ptr NewLine = std::make_shared< Text >( line, Window::_Contents->Width(), _Alignment, ColorDepth_Default );

		_Text.push_back( NewLine );
		Window::_Contents->AddContent( NewLine );

		resetLineSpacing();

		if ( ScrollToNew )
			scrollToLine( NewLine );
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
		Text::Ptr NewLine = std::make_shared< Text >( line, Window::_Contents->Width(), _Alignment, ColorDepth_Default );

		unsigned int CurLine = 0;
		bool Inserted = false;

		// Find the location to insert
		for ( auto l = _Text.begin(), l_end = _Text.end(); l != l_end; ++l )
		{
			if ( ++CurLine >= atLineNum )
			{
				_Text.insert( l, NewLine );
				Inserted = true;
				break;
			}
		}

		// If it wasn't inserted above, we probably ran out of content...just add it to the bottom
		if ( !Inserted )
			_Text.push_back( NewLine );

		Window::_Contents->AddContent( NewLine );
		resetLineSpacing();

		if ( ScrollToNew )
			scrollToLine( NewLine );
	}

	void TextWindow::InsertLineFromBottom( const astring &line, unsigned int atLineNum )
	{
		Text::Ptr NewLine = std::make_shared< Text >( line, Window::_Contents->Width(), _Alignment, ColorDepth_Default );

		unsigned int CurLine = 0;
		bool Inserted = false;

		// Find the location to insert
		for ( auto l = _Text.rbegin(), l_end = _Text.rend(); l != l_end; ++l )
		{
			if ( ++CurLine >= atLineNum )
			{
				_Text.insert( l.base(), NewLine );
				Inserted = true;
				break;
			}
		}

		// If it wasn't inserted above, we probably ran out of content...just add it to the top
		if ( !Inserted )
			_Text.push_front( NewLine );

		Window::_Contents->AddContent( NewLine );
		resetLineSpacing();

		if ( ScrollToNew )
			scrollToLine( NewLine );
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
		unsigned int CurLine = 0;

		// Find the location to remove
		for ( auto l = _Text.begin(), l_end = _Text.end(); l != l_end; ++l )
		{
			if ( ++CurLine > atLineNum )
			{
				for ( unsigned int DelCount = 0; DelCount < numLines; ++DelCount )
				{
					if ( l == l_end )
						break;

					Window::_Contents->RemoveContent( *l );
					_Text.erase( l );
					++l;
				}

				resetLineSpacing();
				break;
			}
		}
	}

	void TextWindow::RemoveLineFromBottom( unsigned int numLines, unsigned int atLineNum )
	{
		unsigned int CurLine = 0;

		// Find the location to remove
		for ( auto l = _Text.rbegin(), l_end = _Text.rend(); l != l_end; ++l )
		{
			if ( ++CurLine > atLineNum )
			{
				for ( unsigned int DelCount = 0; DelCount < numLines; ++DelCount )
				{
					if ( l == l_end )
						break;

					Window::_Contents->RemoveContent( *l );
					_Text.erase( (++l).base() );
					++l;
				}

				resetLineSpacing();
				break;
			}
		}
	}

	void TextWindow::resetLineSpacing()
	{
		Point2D CurPoint( 0, 0 );

		if ( 1 ) //_NewAtBottom )
		{
			for ( auto l = _Text.begin(), l_end = _Text.end(); l != l_end; ++l )
			{
				(*l)->MoveTo( CurPoint );
				CurPoint.Y( CurPoint.Y() + (*l)->Height() );
			}
		}
		else
		{
			for ( auto l = _Text.rbegin(), l_end = _Text.rend(); l != l_end; ++l )
			{
				(*l)->MoveTo( CurPoint );
				CurPoint.Y( CurPoint.Y() + (*l)->Height() );
			}
		}

		RecalculateTotalContentSize();
	}

	void TextWindow::scrollToLine( Text::Ptr line )
	{
		if ( !line )
			return;

		if ( _NewAtBottom )
			MoveViewportTo( Point2D(0, (line->Y() + line->Height()) - _Contents->Height()) );
		else
			MoveViewportTo( Point2D(0, line->Y()) );
	}
}


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --indent-switches --pad-oper


