#include <cstdlib>
#include <ctime>
#include <iostream>

#include "file.hh"
#include "barcode.hh"

using namespace std;

unsigned int paranoid_atoi( const string & in )
{
  const unsigned int ret = stoul( in );
  const string roundtrip = to_string( ret );
  if ( roundtrip != in ) {
    throw runtime_error( "invalid unsigned integer: " + in );
  }
  return ret;
}

int main( int argc, char *argv[] )
{
  /* check arguments */
  if ( argc <= 0 ) { /* for sticklers */
    abort();
  }

  if ( argc != 4 ) {
    cerr << "Usage: " << argv[ 0 ] << " FILE WIDTH HEIGHT\n\n\tNote: thie program writes log file to stderr.\n\n";
    return EXIT_FAILURE;
  }

  /* open file and check for sane length */
  File input { argv[ 1 ] };
  const uint16_t width = paranoid_atoi( argv[ 2 ] );
  const uint16_t height = paranoid_atoi( argv[ 3 ] );
  const size_t frame_length = width * height * sizeof( RGBPixel );

  const size_t frame_count = input.size() / frame_length;

  if ( input.size() != frame_count * frame_length ) {
    throw runtime_error( "file size is not multiple of frame size" );
  } else {
    cerr << "# Reading barcodes from the file: " << argv[ 1 ] <<  ".\n";    
    cerr << "# Found " << frame_count << " frames of size " << width << "x" << height << ".\n";

    std::time_t result = std::time(nullptr);
    cerr << "# Time stamp: " << std::asctime(std::localtime(&result));    
  }

  /* print csv header */
  cerr << "# frame_num" << "," << "upper_left_barcode" << "," << "lower_right_barcode" << "\n";
  
  FileDescriptor stdout { STDOUT_FILENO };
  
  /* iterate through frames and read barcode from each one */
  for ( unsigned int frame_no = 0; frame_no < frame_count; frame_no++ ) {
    const Chunk this_frame_chunk = input( frame_no * frame_length, frame_length );
    XImage this_frame { this_frame_chunk, width, height };

    /* read barcode */
    pair<uint64_t, uint64_t> barcodes = Barcode::readBarcodes( this_frame );
    cerr << frame_no << "," << barcodes.first << "," << barcodes.second << "\n";
  }
  
  return EXIT_SUCCESS;
}
