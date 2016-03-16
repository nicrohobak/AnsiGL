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
		// Need to add something for the input portion itself, or this should literally be only the input portion and nothing else with another class defined that is a combination of a TextWindow and an InputWindow into a InputTextWindow or something similar...
		// Probably need to implement a cursor of some kind...if it could be expanded to also act as a menu cursor, that would be ideal
		//

	public:
		InputWindow();

		virtual std::string Render();
		virtual void RenderToSurface( Surface *dest, const Point &pos = Point(), bool transparentSpaces = TransparentSpacesDefault );
	};
}


#endif // __ANSIGL_INPUTWINDOW_H__


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --pad-oper


