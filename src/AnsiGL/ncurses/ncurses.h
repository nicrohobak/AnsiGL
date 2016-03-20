#ifndef __ANSIGL_NCURSES_H__
#define __ANSIGL_NCURSES_H__

/*
 * ncurses.h
 *
 * A collection of ncurses compatibility functions
 */


#include "../surface.h"
#include "../context.h"

extern "C"
{
	#include <ncursesw/ncurses.h>	// libncursesw for UTF-8 support
}


namespace AnsiGL
{
	//
	// ncurses management
	//
	class ncurses
	{
	public:
		ANSIGL_POINTERS( ncurses )

		//
		// ncurses has color limitations in terms of how many can be displayed at one time.
		// This is a palette to allow ncurses COLOR_PAIR()'s to be created on-the-fly.
		//
		// NOTE: Be warned!  In ncurses mode there are limitations based on your terminal type!
		//       If things aren't acting as expected, verify capabilities with terminfo!
		//
		typedef std::map< ColorDef::Ptr, int >	tColorPalette;
		static tColorPalette					ColorPalette;
		static constexpr short					DEFAULT_COLOR_PAIR = 1;

	public:
		static int ColorPair( const ColorDef &color );		// Returns the COLOR_PAIR(x) for this ColorDef
		static void RenderSurface( Surface::Ptr src, WINDOW *dest, Point2D pos = Point2D(0,0), bool transparentSpaces = TRANSPARENT_DEFAULT );
		static attr_t Attributes( const achar &ach );		// Returns the appropriate attributes within ncurses for this achar

	public:
		ncurses( bool setLocale = true )					// Makes sure to set the locale before starting ncurses
		{
			if ( setLocale )
				setlocale( LC_ALL, "" );

			initscr();
			start_color();

			raw();
			keypad( stdscr, TRUE );
			noecho();
			curs_set( 0 );
			mousemask( ALL_MOUSE_EVENTS, NULL );

			_Screen = std::make_shared< Surface >( Size() );

			if ( !has_colors() )
				_Screen->RenderANSI = false;
		}

		~ncurses()
		{
			endwin();
		}

		Area2D Size() const
		{
			return Area2D( COLS, LINES );
		}

		void Refresh( Context::Ptr content = NULL )
		{
			_Screen->Clear();

			if ( content )
				content->RenderToSurface( _Screen );

			RenderSurface( _Screen, stdscr );
			refresh();
		}

		int GetCh()
		{
			return getch();
		}

	protected:
		Surface::Ptr	_Screen;
	};
}


#endif // __ANSIGL_NCURSES_H__


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --pad-oper


