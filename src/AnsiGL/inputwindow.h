#ifndef __ANSIGL_INPUTWINDOW_H__
#define __ANSIGL_INPUTWINDOW_H__

/*
 * inputwindow.h
 *
 * A Window with an additional "input" area (similar to a traditional IM window).  The input area is compact, but optionally has borders, and it can roll out over the top of the window contents as more content builds up in the input area.
 */


#include "textwindow.h"


namespace AnsiGL
{
	class InputWindow : public TextWindow
	{
	public:
		ANSIGL_POINTERS( InputWindow )

	public:
		InputWindow():
			_Input( std::make_shared< Text >("") ),
			_Cursor( std::make_shared< Text >("_") ),
			_Prompt( std::make_shared< Text >(">") )
		{
			_Input->Width( _Layout->Width() - 2 );
			_Layout->AddContent( _Input, Point3D(0, 0, 5) );
			_Layout->AddContent( _Cursor, Point3D(0, _Layout->Height() - 1, 5) );
			_Layout->AddContent( _Prompt, Point3D(0, _Layout->Height() - 1, 5) );
			this->updateWindow();
		}

		InputWindow( const astring &windowTitle,
					 const Area2D &windowSize = Area2D(80, 24),
					 unsigned int maxLines = DEFAULT_MAX_LINES,
					 ENUM_TxtAlign alignment = TxtAlign_Default,
					 bool newAtBottom = true,
					 const Point3D &viewportPos = Point3D(), bool transparentSpaces = false ):
			TextWindow( windowTitle, windowSize, maxLines, alignment, newAtBottom, viewportPos, transparentSpaces ),
			_Input( std::make_shared< Text >("") ),
			_Cursor( std::make_shared< Text >("_") ),
			_Prompt( std::make_shared< Text >(">") )
		{
			_Input->Width( _Layout->Width() - 2 );
			_Layout->AddContent( _Input, Point3D(0, 0, 5) );
			_Layout->AddContent( _Cursor, Point3D(0, _Layout->Height() - 1, 5) );
			_Layout->AddContent( _Prompt, Point3D(0, _Layout->Height() - 1, 5) );
			this->updateWindow();
		}

		virtual ~InputWindow()
		{
		}

		virtual void Clear();
		virtual void ClearContents();
		virtual void ClearInput();

		virtual const Text &CurInput() const;

		virtual void InputChar( const achar &ch );			// Adds a character to the input
		virtual void InputLine( const astring &line );		// Appends a line to the input

	protected:
		Text::Ptr		_Input;
		Text::Ptr		_Cursor;
		Text::Ptr		_Prompt;

	protected:
		virtual void updateWindow();
	};
}


#endif // __ANSIGL_INPUTWINDOW_H__


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --pad-oper


