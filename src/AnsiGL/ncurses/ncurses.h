#ifndef __ANSIGL_NCURSES_H__
#define __ANSIGL_NCURSES_H__

/*
 * ncurses.h
 *
 * A collection of ncurses compatibility functions
 */


#include "../surface.h"

extern "C"
{
#include <ncursesw/ncurses.h>	// libncursesw for UTF-8 support
}


namespace AnsiGL
{
	//
	// Functions to start or stop Ncurses (NOTE: There is currently nothing special in these functions...they are currently optional, but may not always be!)
	//
	void StartNcurses( bool setLocale = true );				// Makes sure to set the locale before starting ncurses, unless overridden
	void StopNcurses();


	//
	// An ncurses COLOR_PAIR() lookup
	//
	int NcursesColorPair( const ColorDef &color );			// Returns the COLOR_PAIR(x) for this ColorDef


	//
	// ncurses compatible rendering functions
	//
	void RenderSurfaceToNcurses( Surface::Ptr src, WINDOW *dest, Point pos = Point(0, 0), bool transparentSpaces = TRANSPARENT_DEFAULT );
	attr_t NcursesAttributes( const achar &ach );			// Returns the appropriate attributes within ncurses for this achar
}


#endif // __ANSIGL_NCURSES_H__


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --pad-oper


