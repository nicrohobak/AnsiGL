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
			_Cursor->Visible( true );
			_Prompt->Visible( true );
		}
		else
		{
			_Input->Visible( false );
			_Cursor->Visible( false );
			_Prompt->Visible( false );
		}
	}

	bool InputWindow::CapturingColor() const
	{
		return _CaptureColor;
	}

	void InputWindow::CancelColorCapture()
	{
		// Pull the escape code out of the UI
		while ( _RemoveChars-- > 0 )
			_Input->pop_back();

		_RemoveChars = 0;

		_ColorBuffer.clear();
		_CaptureColor = false;
		_CaptureColorList = false;

		this->updateWindow();
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
		_CurColor = achar();
		setCursorColor();
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

		char Glyph = ch.Glyph().c_str()[0];
		achar NewCh = ch;
		NewCh.Color = _CurColor.Color;

		// Special buffering for color sequences
		if ( _CaptureColor )
		{
			switch ( Glyph )
			{
				case 8:
				case 127:
					_Input->pop_back();
					_ColorBuffer.pop_back();
					--_RemoveChars;

					if ( _ColorBuffer.empty() )
					{
						_CaptureColor = false;
						_CaptureColorList = false;
						_RemoveChars = 0;
					}

					this->updateWindow();
					break;

				// If we try to terminate the color too early, just cancel the color altogether
				case '\n':
					CancelColorCapture();
					break;

				case COLOR_LIST_START_CHAR:
					_ColorBuffer.push_back( Glyph );
					_CaptureColorList = true;

					// Update our UI, and make note that we should remove this character later
					*_Input << Glyph;
					++_RemoveChars;
					this->updateWindow();
					break;

				case COLOR_LIST_END_CHAR:
					{
						_ColorBuffer.push_back( Glyph );

						achar ColorCh( _ColorBuffer );
						_CurColor.Color = ColorCh.Color;
						setCursorColor();

						CancelColorCapture();
						break;
					}

				default:
					{
						_ColorBuffer.push_back( Glyph );

						if ( _CaptureColorList )
						{
							// Update our UI, and make note that we should remove this character later
							*_Input << Glyph;
							++_RemoveChars;
							this->updateWindow();
							break;
						}

						achar ColorCh( _ColorBuffer );
						_CurColor.Color = ColorCh.Color;
						setCursorColor();

						CancelColorCapture();
						break;
					}
			}

			return;
		}

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

			// Color code handling
			case COLOR_ESCAPE_CHAR:
				_ColorBuffer = "^";
				NewCh.Color.Clear();
				*_Input << NewCh;
				++_RemoveChars;
				_CaptureColor = true;
				break;

			default:
				*_Input << NewCh;
				break;
		}

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
		_Cursor->MoveTo( Point2D((_Input->Length() % _Input->Width()) + 1, _Layout->Height() - 1) );
	}

	void InputWindow::setCursorColor()
	{
		// Set the cursor to the current active color
		for ( auto c = _Cursor->begin(), c_end = _Cursor->end(); c != c_end; ++c )
			c->Color = _CurColor.Color;

		_Cursor->Format();
	}
}


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --indent-switches --pad-oper


