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
		_Input->MoveTo( Point2D(0, _Layout->Height() - _Input->Height() - ExtraLineSpacing) );

		// And move our cursor to where it's needed
		_Cursor->MoveTo( Point2D(_Input->LastLineLength(), _Layout->Height() - 1) );
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

	void InputText::SetColorCodes( const tColorCode *colorCodes )
	{
		if ( colorCodes )
			_ColorCodes = colorCodes;
		else
			_ColorCodes = ActiveColorCodes;
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
			// Sacrificing the switch for flexibility
			if ( Glyph == 8
			  || Glyph == 127 )
			{
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
			}
			// If we try to terminate the color too early, just cancel the color altogether
			else if ( Glyph == '\n' )
			{
				CancelColorCapture();
			}
			else if ( _ColorCodes && Glyph == _ColorCodes[ColorCode_List_Start] )
			{
				_ColorBuffer.push_back( Glyph );
				_CaptureColorList = true;

				// Update our UI, and make note that we should remove this character later
				*_Input << Glyph;
				++_RemoveChars;
				this->Format();
			}
			else if ( _ColorCodes && Glyph == _ColorCodes[ColorCode_List_End] )
			{
				_ColorBuffer.push_back( Glyph );

				achar ColorCh( _ColorBuffer );
				_CurColor.Color = ColorCh.Color;
				setCursorColor();

				CancelColorCapture();
			}
			else
			{
				_ColorBuffer.push_back( Glyph );

				if ( _CaptureColorList )
				{
					// Update our UI, and make note that we should remove this character later
					*_Input << Glyph;
					++_RemoveChars;
					this->Format();
				}

				achar ColorCh( _ColorBuffer, _CurColor, _ColorCodes );
				_CurColor.Color = ColorCh.Color;
				setCursorColor();

				CancelColorCapture();
			}

			return;
		}

		// Sacrificing the switch for flexibility
		// Skip if it's NULL
		if ( Glyph == 0 )
		{
			return;
		}
		// Backspace / delete
		else if ( Glyph == 8
			   || Glyph == 127 )
		{
			_Input->pop_back();
		}
		// Color code handling
		else if ( _ColorCodes && Glyph == _ColorCodes[ColorCode_Escape] )
		{
			_ColorBuffer.push_back( _ColorCodes[ColorCode_Escape] );
			NewCh.Color.Clear();
			*_Input << NewCh;
			++_RemoveChars;
			_CaptureColor = true;
		}
		else
			*_Input << NewCh;

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


