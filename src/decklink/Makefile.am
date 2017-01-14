AM_CPPFLAGS = $(CXX14_FLAGS) -I$(srcdir)/../util -I$(srcdir)/../display $(XCBPRESENT_CFLAGS) $(XCB_CFLAGS) $(CXX14_FLAGS)
AM_CXXFLAGS = $(PICKY_CXXFLAGS)

noinst_LIBRARIES = libdecklink.a

libdecklink_a_SOURCES = DeckLinkCapturer.hh DeckLinkMixin.hh DeckLinkPlayer.hh \
						DeckLinkCapturer.cc DeckLinkMixin.cc DeckLinkPlayer.cc
