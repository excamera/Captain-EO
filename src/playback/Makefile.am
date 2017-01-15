AM_CPPFLAGS = -I$(srcdir)/../../third_party/decklink -I$(srcdir)/../util -I$(srcdir)/../display -I$(srcdir)/../scanner -I$(srcdir)/../barcoder -I$(srcdir)/../util $(XCBPRESENT_CFLAGS) $(XCB_CFLAGS) $(CXX14_FLAGS)
AM_CXXFLAGS = $(PICKY_CXXFLAGS)

bin_PROGRAMS = playback
playback_SOURCES = Playback.cc Playback.hh Config.h Config.cpp
playback_LDADD = ../../third_party/decklink/libdecklink.a -ldl -lm ../display/libdisplay.a ../scanner/libscanner.a ../barcoder/libbarcode.a ../util/libutil.a $(XCBPRESENT_LIBS) $(XCB_LIBS) $(PANGOCAIRO_LIBS)
playback_LDFLAGS = -pthread
