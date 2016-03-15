INCLUDE_DIRS = -I$(top_srcdir) -I$(top_srcdir)/src -I$(top_srcdir)/src/AnsiGL
ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES = libAnsiGL_ncurses.la

libAnsiGL_ncurses_la_SOURCES =	ncurses.cpp

libAnsiGL_ncurses_la_CXXFLAGS = $(INCLUDE_DIRS) -std=c++14 -pedantic -Wall -pg
#libAnsiGL_ncurses_la_CXXFLAGS = $(INCLUDE_DIRS) -std=c++14 -pedantic -Wall

library_includedir = $(includedir)/AnsiGL/ncurses
library_include_HEADERS =	$(top_srcdir)/src/AnsiGL/ncurses/ncurses.h



