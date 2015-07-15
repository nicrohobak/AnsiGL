/*
 * inputwindow.cpp
 *
 * A Window with an additional "input" area (similar to a traditional IM window).  The input area is compact, but optionally has borders and can expand in height to roll out over the window contents as more content builds up in the input area.
 */


#include <cmath>

#include "AnsiGL/inputwindow.h"


namespace AnsiGL
{
    InputWindow::InputWindow()
    {
    }

    std::string InputWindow::Render()
    {
	return TextWindow::Render();
    }

    void InputWindow::RenderToSurface( Surface *dest, const Point &pos, bool transparentSpaces )
    {
	TextWindow::RenderToSurface( dest, pos, transparentSpaces );
    }
}





