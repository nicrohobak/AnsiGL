INCLUDE_DIRS = -I$(top_srcdir) -I/devhome/INSTALL/include

LIB_DIRS = -L$(top_srcdir) -L/devhome/INSTALL/lib


bin_PROGRAMS = ansigl_unit_tests

ansigl_unit_tests_SOURCES =	main.cpp			\
				test_achar.cpp			\
				test_ANSICodeList.cpp		\
				test_ANSIColorDef.cpp		\
				test_ANSIColorPalette.cpp	\
				test_Area.cpp			\
				test_astring.cpp		\
				test_ColorComponent.cpp		\
				test_Color.cpp			\
				test_ColorPalette.cpp		\
				test_FixedArea.cpp		\
				test_Pixel.cpp			\
				test_Point.cpp			\
				test_Scanline.cpp		\
				test_Surface.cpp		\
				test_uchar.cpp			\
				test_ustring.cpp


ansigl_unit_tests_CXXFLAGS = $(INCLUDE_DIRS) -Wall -Wno-unused-variable
ansigl_unit_tests_LDADD = $(LIB_DIRS) -lansigl



