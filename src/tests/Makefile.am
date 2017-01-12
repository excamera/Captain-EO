AM_CPPFLAGS = $(CXX14_FLAGS)
AM_CXXFLAGS = $(PICKY_CXXFLAGS)

dist_check_SCRIPTS = fetch-vectors.test barcode-roundtrip.test

TESTS = fetch-vectors.test barcode-roundtrip.test

barcode-roundtrip.log: fetch-vectors.log

clean-local:
	-rm -rf captain-eo-test-vectors
	-rm -f barcoded.john.truncated.raw barcodes-read.log barcodes-read.stdoutlog barcodes-written.log
