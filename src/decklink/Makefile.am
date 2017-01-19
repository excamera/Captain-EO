AM_CPPFLAGS = $(CXX14_FLAGS) -I$(srcdir)/../util  -I$(srcdir)/../../third_party/decklink -I$(srcdir)/../display $(XCBPRESENT_CFLAGS) $(XCB_CFLAGS) $(CXX14_FLAGS)
AM_CXXFLAGS = $(PICKY_CXXFLAGS)

noinst_LIBRARIES = libdecklinkshim.a

libdecklinkshim_a_SOURCES = DeckLinkCapturer.hh DeckLinkMixin.hh DeckLinkPlayer.hh \
						DeckLinkCapturer.cc DeckLinkMixin.cc DeckLinkPlayer.cc
