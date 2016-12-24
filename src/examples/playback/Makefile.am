AM_CPPFLAGS = -I$(srcdir)/../../../third_party/decklink -I$(srcdir)/../../display $(XCBPRESENT_CFLAGS) $(XCB_CFLAGS) $(PANGOCAIRO_CFLAGS) $(CXX14_FLAGS)
AM_CXXFLAGS = $(PICKY_CXXFLAGS)

bin_PROGRAMS = playback
playback_SOURCES = TestPattern.cpp TestPattern.h Config.h Config.cpp ../scanner/Scanner.hh
playback_LDADD = ../../../third_party/decklink/libdecklink.a -ldl -lm
playback_LDFLAGS = -pthread
