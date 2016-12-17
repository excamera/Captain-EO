AM_CPPFLAGS = -I$(srcdir)/../../../third_party/decklink -I$(srcdir)/../../display $(XCBPRESENT_CFLAGS) $(XCB_CFLAGS) $(PANGOCAIRO_CFLAGS) $(CXX14_FLAGS)
AM_CXXFLAGS = $(PICKY_CXXFLAGS)

bin_PROGRAMS = test-pattern
test_pattern_SOURCES = TestPattern.cpp TestPattern.h VideoFrame3D.cpp VideoFrame3D.h Config.h Config.cpp
test_pattern_LDADD = ../../../third_party/decklink/libdecklink.a -ldl -lm
test_pattern_LDFLAGS = -pthread
