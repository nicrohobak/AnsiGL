/*
 * inputwindow.cpp
 *
 * A Window with an additional "input" area (similar to a traditional IM window).  The input area is compact, but optionally has borders and can expand in height to roll out over the window contents as more content builds up in the input area.
 */


#include <cmath>

#include "AnsiGL/inputwindow.h"


namespace AnsiGL
{
	bool InputWindow::Enabled() const
	{
		return _Enabled;
	}

	void InputWindow::Enabled( bool enabled )
	{
		_Enabled = enabled;

		if ( _Enabled )
		{
			_Input->Visible( true );
			_Prompt->Visible( true );

			if ( !_Input->empty() )
				_Cursor->Visible( true );
		}
		else
		{
			_Input->Visible( false );
			_Cursor->Visible( false );
			_Prompt->Visible( false );
		}
	}

	void InputWindow::Clear()
	{
		this->ClearInput();
		this->ClearContents();
	}

	void InputWindow::ClearContents()
	{
		TextWindow::Clear();
	}

	void InputWindow::ClearInput()
	{
		_Input->Clear();
		_Cursor->MoveTo( Point2D(0, _Layout->Height() - 1) );
		_Cursor->Visible( false );
		this->updateWindow();
	}

	const Text &InputWindow::CurInput() const
	{
		return *_Input;
	}

	void InputWindow::InputChar( const achar &ch )
	{
		if ( !_Enabled )
			return;

		achar NewCh = ch;
		char Glyph = NewCh.Glyph().c_str()[0];

		switch ( Glyph )
		{
			// Skip if it's NULL
			case 0:
				return;

			// Backspace / delete
			case 8:
			case 127:
				_Input->pop_back();
				break;

			default:
				NewCh.Color.FG = AnsiGL::ANSI_FG_Black;
				NewCh.Color.BG = AnsiGL::ANSI_BG_White;
				*_Input << NewCh;
				break;
		}
	
		if ( _Input->empty() )
			_Cursor->Visible( false );
		else
			_Cursor->Visible( true );

		this->updateWindow();
	}

	void InputWindow::InputLine( const astring &line )
	{
		if ( !_Enabled )
			return;

		*_Input << line;
		this->updateWindow();
	}

	void InputWindow::updateWindow()
	{
		int ExtraLineSpacing = 0;

		if ( !(_Input->Length() % _Contents->Width()) )
			++ExtraLineSpacing;

		// Put the input at the bottom of the window, on top of the border
		_Input->MoveTo( Point2D(1, _Layout->Height() - _Input->Height() - ExtraLineSpacing) );

		// And move our cursor to where it's needed
		unsigned int CursorX = 0;

		if ( !_Input->empty() )
			CursorX = (_Input->Length() % _Input->Width()) + 1;

		_Cursor->MoveTo( Point2D(CursorX, _Layout->Height() - 1) );
	}
}


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --pad-oper


