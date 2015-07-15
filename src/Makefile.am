SUBDIRS = ncurses
INCLUDE_DIRS = -I$(top_srcdir) -I$(top_srcdir)/src
ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES = libAnsiGL.la

libAnsiGL_la_SOURCES =  achar.cpp		\
			animation.cpp		\
			ansi.cpp		\
			area.cpp		\
			camera.cpp		\
			clone.cpp		\
			color.cpp		\
			content.cpp		\
			context.cpp		\
			image.cpp		\
			pixel.cpp		\
			point.cpp		\
			sprite.cpp		\
			string.cpp		\
			surface.cpp		\
			text.cpp		\
			textwindow.cpp		\
			uchar.cpp		\
			window.cpp

#			inputwindow.cpp		


libAnsiGL_la_CXXFLAGS = $(INCLUDE_DIRS) -Wall -pg
#libAnsiGL_la_CXXFLAGS = $(INCLUDE_DIRS) -Wall

library_includedir = $(includedir)/AnsiGL
library_include_HEADERS =	AnsiGL/achar.h		\
				AnsiGL/animation.h	\
				AnsiGL/ansi.h		\
				AnsiGL/area.h		\
				AnsiGL/camera.h		\
				AnsiGL/clone.h		\
				AnsiGL/color.h		\
				AnsiGL/content.h	\
				AnsiGL/context.h	\
				AnsiGL/image.h		\
				AnsiGL/pixel.h		\
				AnsiGL/point.h		\
				AnsiGL/sprite.h		\
				AnsiGL/string.h		\
				AnsiGL/surface.h	\
				AnsiGL/text.h		\
				AnsiGL/textwindow.h	\
				AnsiGL/uchar.h		\
				AnsiGL/window.h


#				AnsiGL/inputwindow.h		




