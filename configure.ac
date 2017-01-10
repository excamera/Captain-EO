#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([captain-eo], [0.1], [excamera@cs.stanford.edu])
AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_SRCDIR([src/display/display.hh])
AC_CONFIG_HEADERS([config.h])

# Add picky CXXFLAGS
CXX14_FLAGS="-std=c++14 -pthread"
PICKY_CXXFLAGS="-pedantic -Wall -Wno-multichar -Wextra -Weffc++"
AC_SUBST([CXX14_FLAGS])
AC_SUBST([PICKY_CXXFLAGS])

# Checks for programs.
AC_PROG_CXX
AC_PROG_RANLIB

# Checks for libraries.
PKG_CHECK_MODULES([XCB], [xcb])
PKG_CHECK_MODULES([XCBPRESENT], [xcb-present])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile
         third_party/Makefile
         third_party/decklink/Makefile
         third_party/daala_tools/Makefile
         src/Makefile
         src/display/Makefile
         src/examples/Makefile
         src/examples/playback/Makefile
         src/examples/rgb-example/Makefile
         src/examples/capture/Makefile
         src/examples/barcoder/Makefile
         src/examples/scanner/Makefile
         ])
AC_OUTPUT
