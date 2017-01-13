AM_CPPFLAGS = -I$(srcdir)/../../third_party/decklink -I$(srcdir)/../util -I$(srcdir)/../display -I$(srcdir)/../scanner $(XCBPRESENT_CFLAGS) $(XCB_CFLAGS) $(CXX14_FLAGS)
AM_CXXFLAGS = $(PICKY_CXXFLAGS)

bin_PROGRAMS = capture
capture_SOURCES = Capture.cc Capture.hh Config.hh Config.cc ../barcoder/barcode.hh ../barcoder/barcode.cc
capture_LDADD = ../../third_party/decklink/libdecklink.a -ldl -lm ../display/libdisplay.a ../scanner/libscanner.a $(XCBPRESENT_LIBS) $(XCB_LIBS) $(PANGOCAIRO_LIBS)
capture_LDFLAGS = -pthread
