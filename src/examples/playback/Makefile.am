AM_CPPFLAGS = -I$(srcdir)/../../../third_party/decklink -I$(srcdir)/../../display -I$(srcdir)/../scanner $(XCBPRESENT_CFLAGS) $(XCB_CFLAGS) $(PANGOCAIRO_CFLAGS) $(CXX14_FLAGS)
AM_CXXFLAGS = $(PICKY_CXXFLAGS)

bin_PROGRAMS = playback
playback_SOURCES = Playback.cc Playback.hh Config.h Config.cpp 
playback_LDADD = ../../../third_party/decklink/libdecklink.a -ldl -lm ../scanner/libscanner.a 
playback_LDFLAGS = -pthread
