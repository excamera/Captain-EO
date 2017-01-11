AM_CPPFLAGS = -I$(srcdir)/../display $(XCBPRESENT_CFLAGS) $(XCB_CFLAGS) $(PANGOCAIRO_CFLAGS) $(CXX14_FLAGS)
AM_CXXFLAGS = $(PICKY_CXXFLAGS)

bin_PROGRAMS = barcoder
barcoder_SOURCES = main.cpp barcoder.hh barcoder.cpp
barcoder_LDADD = ../display/libdisplay.a $(XCBPRESENT_LIBS) $(XCB_LIBS)
