AM_CPPFLAGS = -I$(srcdir)/../util -I$(srcdir)/../display $(XCBPRESENT_CFLAGS) $(XCB_CFLAGS) $(CXX14_FLAGS)
AM_CXXFLAGS = $(PICKY_CXXFLAGS)

noinst_LIBRARIES = libbarcode.a

libbarcode_a_SOURCES = barcode.hh barcode.cc

bin_PROGRAMS = barcode-write
barcode_write_SOURCES = barcode-write.cc
barcode_write_LDADD = libbarcode.a ../util/libutil.a ../display/libdisplay.a $(XCBPRESENT_LIBS) $(XCB_LIBS)

bin_PROGRAMS += barcode-read
barcode_read_SOURCES = barcode-read.cc
barcode_read_LDADD = libbarcode.a ../util/libutil.a ../display/libdisplay.a $(XCBPRESENT_LIBS) $(XCB_LIBS)
