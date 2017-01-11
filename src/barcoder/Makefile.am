AM_CPPFLAGS = -I$(srcdir)/../util -I$(srcdir)/../display $(XCBPRESENT_CFLAGS) $(XCB_CFLAGS) $(CXX14_FLAGS)
AM_CXXFLAGS = $(PICKY_CXXFLAGS)

# bin_PROGRAMS = barcoder
# barcoder_SOURCES = main.cc barcoder.hh barcoder.cc
# barcoder_LDADD = ../display/libdisplay.a $(XCBPRESENT_LIBS) $(XCB_LIBS)

bin_PROGRAMS = barcode-write
barcode_write_SOURCES = barcode-write.cc barcoder.hh barcoder.cc
barcode_write_LDADD = ../util/libutil.a ../display/libdisplay.a $(XCBPRESENT_LIBS) $(XCB_LIBS)

bin_PROGRAMS += barcode-read
barcode_write_SOURCES = barcode-write.cc barcoder.hh barcoder.cc
barcode_write_LDADD = ../util/libutil.a ../display/libdisplay.a $(XCBPRESENT_LIBS) $(XCB_LIBS)
