#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([ansigl], [0.1], [nicro.hobak@gmail.com])
AM_INIT_AUTOMAKE([-Wall])
LT_INIT([])
AC_CONFIG_SRCDIR([src/surface.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET

# Make sure we can compile c++14
AX_CXX_COMPILE_STDCXX_14

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_SIZE_T

# Checks for library functions.

AC_CONFIG_FILES([Makefile
		src/Makefile
		src/ncurses/Makefile])

AC_OUTPUT
LT_OUTPUT
