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
			_Prompt->Visible( true );
		else
			_Prompt->Visible( false );

		_Input->Enabled( _Enabled );
	}

	bool InputWindow::CapturingColor() const
	{
		return _Input->CapturingColor();
	}

	void InputWindow::CancelColorCapture()
	{
		_Input->CancelColorCapture();
	}

	void InputWindow::SetColorCodes( const tColorCode *colorCodes )
	{
		_Input->SetColorCodes( colorCodes );
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
	}

	const InputText &InputWindow::CurInput() const
	{
		return *_Input;
	}

	void InputWindow::InputChar( const achar &ch )
	{
		if ( !_Enabled )
			return;

		_Input->InputChar( ch );
	}

	void InputWindow::InputLine( const astring &line )
	{
		if ( !_Enabled )
			return;

		_Input->InputLine( line );
	}
}


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --indent-switches --pad-oper


