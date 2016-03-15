#!/bin/bash

#
# TODO: This should probably get a proper autotools makefile setup, etc.
#       This is the kludge that does it for now.
#

APP_NAME="AnsiGL_test"

INCLUDE_DIR="/path/to/include/dir"
LIB_DIR="/path/to/lib/dir"

ANSIGL="$LIB_DIR/libAnsiGL.a"
ANSIGL_NCURSES="$LIB_DIR/libAnsiGL_ncurses.a"

# NOTE: ncursesw is now required in order to support unicode characters!  -lncurses probably will not link!
OTHER_LIBS="-lncursesw"

# The top line is for profiling builds
#g++ -std=c++14 -pedantic -pg -o $APP_NAME -I$INCLUDE_DIR main.cpp $ANSIGL $ANSIGL_NCURSES $OTHER_LIBS
g++ -std=c++14 -pedantic -o $APP_NAME -I$INCLUDE_DIR main.cpp $ANSIGL $ANSIGL_NCURSES $OTHER_LIBS


