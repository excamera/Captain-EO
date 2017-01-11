AM_CPPFLAGS = -I$(srcdir)/../util -I$(srcdir)/../display $(XCBPRESENT_CFLAGS) $(XCB_CFLAGS) $(CXX14_FLAGS)
AM_CXXFLAGS = $(PICKY_CXXFLAGS)

noinst_LIBRARIES = libscanner.a

libscanner_a_SOURCES = Scanner.hh Scanner.cpp

bin_PROGRAMS = scanner-test
scanner_test_SOURCES = ScannerTest.cpp
scanner_test_LDADD = libscanner.a
