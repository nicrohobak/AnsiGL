/*
 * inputtext.cpp
 *
 * A Text object that accepts user input.
 */


#include "AnsiGL/inputtext.h"


namespace AnsiGL
{
	const astring &InputText::Value() const
	{
		return _Input->Value();
	}

	const astring &InputText::FormattedValue() const
	{
		return _Input->FormattedValue();
	}

	bool InputText::empty() const
	{
		return _Input->empty();
	}

	ENUM_TxtAlign InputText::Align()  const
	{
		return _Input->Align();
	}

	void InputText::Align( ENUM_TxtAlign align )
	{
		_Input->Align( align );
	}

	tSizeType InputText::Length() const
	{
		return _Input->Length();
	}

	bool InputText::Enabled() const
	{
		return _Enabled;
	}

	void InputText::Enabled( bool enabled )
	{
		_Enabled = enabled;

		if ( _Enabled )
		{
			_Input->Visible( true );
			_Cursor->Visible( true );
		}
		else
		{
			_Input->Visible( false );
			_Cursor->Visible( false );
		}
	}

	void InputText::Format()
	{
		_Input->Format();

		int ExtraLineSpacing = 0;

		if ( !(_Input->Length() % _Layout->Width()) )
			++ExtraLineSpacing;

		// Put the input at the bottom of the window, on top of the border
		_Input->MoveTo( Point2D(1, _Layout->Height() - _Input->Height() - ExtraLineSpacing) );

		// And move our cursor to where it's needed
		_Cursor->MoveTo( Point2D((_Input->Length() % _Input->Width()) + 1, _Layout->Height() - 1) );
	}

	bool InputText::CapturingColor() const
	{
		return _CaptureColor;
	}

	void InputText::CancelColorCapture()
	{
		// Pull the escape code out of the UI
		while ( _RemoveChars-- > 0 )
			_Input->pop_back();

		_RemoveChars = 0;

		_ColorBuffer.clear();
		_CaptureColor = false;
		_CaptureColorList = false;

		this->Format();
	}

	void InputText::Clear()
	{
		_Input->Clear();
		_Cursor->MoveTo( Point2D(0, _Layout->Height() - 1) );
		_CurColor = achar();
		setCursorColor();
		this->Format();
	}

	void InputText::InputChar( const achar &ch )
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

					this->Format();
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
					this->Format();
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
							this->Format();
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

		this->Format();
	}

	void InputText::InputLine( const astring &line )
	{
		if ( !_Enabled )
			return;

		*_Input << line;
		this->Format();
	}

	void InputText::Width( tSizeType width )
	{
		_Layout->Width( width );
		_Layout->ResizeViewport( Area2D(width, _Layout->Height()) );
		_Input->Width( width );
		_Input->Format();
		this->Format();
	}

	void InputText::Height( tSizeType height )
	{
		_Layout->Height( height );
		_Layout->ResizeViewport( Area2D(_Layout->Width(), height) );
		this->Format();
	}

	void InputText::Resize( const Area2D &size )
	{
		_Layout->Resize( size );
		_Layout->ResizeViewport( size );
		_Input->Width( size.Width() );
		_Input->Format();
		this->Format();
	}

	std::string InputText::str()
	{
		return _Layout->str();
	}

	std::string InputText::Render() const
	{
		return _Layout->Render();
	}

	void InputText::RenderToSurface( Surface::Ptr dest, const Point2D &pos ) const
	{
		_Layout->RenderToSurface( dest, pos );
	}

	void InputText::setCursorColor()
	{
		// Set the cursor to the current active color
		for ( auto c = _Cursor->begin(), c_end = _Cursor->end(); c != c_end; ++c )
			c->Color = _CurColor.Color;

		_Cursor->Format();
	}
}


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --indent-switches --pad-oper


