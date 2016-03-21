#ifndef __ANSIGL_INPUTWINDOW_H__
#define __ANSIGL_INPUTWINDOW_H__

/*
 * inputwindow.h
 *
 * A Window with an additional "input" area (similar to a traditional IM window).  The input area is compact, but optionally has borders, and it can roll out over the top of the window contents as more content builds up in the input area.
 */


#include "inputtext.h"
#include "textwindow.h"


namespace AnsiGL
{
	class InputWindow : public TextWindow
	{
	public:
		ANSIGL_POINTERS( InputWindow )

	public:
		InputWindow():
			_Enabled( true ),
			_Input( std::make_shared< InputText >(_Layout->Width() - 2) ),
			_Prompt( std::make_shared< Text >(">") )
		{
			_Input->Width( _Layout->Width() - 2 );
			_Layout->AddContent( _Input, Point3D(0, _Layout->Height() - 1, 5) );
			_Layout->AddContent( _Prompt, Point3D(0, _Layout->Height() - 1, 5) );
		}

		InputWindow( const astring &windowTitle,
					 const Area2D &windowSize = Area2D(80, 24),
					 unsigned int maxLines = DEFAULT_MAX_LINES,
					 ENUM_TxtAlign alignment = TxtAlign_Default,
					 bool newAtBottom = true,
					 const Point3D &viewportPos = Point3D(), bool transparentSpaces = false ):
			TextWindow( windowTitle, windowSize, maxLines, alignment, newAtBottom, viewportPos, transparentSpaces ),
			_Enabled( true ),
			_Input( std::make_shared< InputText >(_Layout->Width() - 2) ),
			_Prompt( std::make_shared< Text >(">") )
		{
			_Input->Width( _Layout->Width() - 2 );
			_Layout->AddContent( _Input, Point3D(0, _Layout->Height() - 1, 5) );
			_Layout->AddContent( _Prompt, Point3D(0, _Layout->Height() - 1, 5) );
		}

		virtual ~InputWindow()
		{
		}

		virtual bool Enabled() const;
		virtual void Enabled( bool enabled );

		bool CapturingColor() const;						// Returns 'true' if we're currently capturing a color sequence
		void CancelColorCapture();							// Cancels the current, in-progress color sequence

		virtual void Clear();
		virtual void ClearContents();
		virtual void ClearInput();

		virtual const InputText &CurInput() const;

		virtual void InputChar( const achar &ch );			// Adds a character to the input
		virtual void InputLine( const astring &line );		// Appends a line to the input

	protected:
		bool			_Enabled;

		InputText::Ptr	_Input;
		Text::Ptr		_Prompt;
	};
}


#endif // __ANSIGL_INPUTWINDOW_H__


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --indent-switches --pad-oper


