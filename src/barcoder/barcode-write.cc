#include <cstdlib>
#include <iostream>
#include <limits>
#include <random>

#include "file.hh"
#include "barcode.hh"

/* #define _BARCODE_WRITE_DEBUG */

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
    cerr << "Usage: " << argv[ 0 ] << " FILE WIDTH HEIGHT\n\n\tNOTE: this program...\n\t(1) writes barcoded image to stdout.\n\t(2) writes log file to stderr.\n\n";
    return EXIT_FAILURE;
  }

  /* open file and check for sane length */
  File input { argv[ 1 ] };
  const unsigned int width = paranoid_atoi( argv[ 2 ] );
  const unsigned int height = paranoid_atoi( argv[ 3 ] );
  const unsigned int frame_length = width * height * sizeof( RGBPixel );

  const unsigned int frame_count = input.size() / frame_length;
  if ( input.size() != frame_count * frame_length ) {
    throw runtime_error( "file size is not multiple of frame size" );
  } else {
    #ifdef _BARCODE_WRITE_DEBUG
    cerr << "Found " << frame_count << " frames of size " << width << "x" << height << ".\n";
    #endif
  }

  FileDescriptor stdout { STDOUT_FILENO };

  /* initialize random number generator */
  random_device rd;
  mt19937 generator(rd());
  uniform_int_distribution<uint64_t> uniform_distribution(0, numeric_limits<uint64_t>::max());
  
  /* iterate through frames and add barcode to each one */
  for ( unsigned int frame_no = 0; frame_no < frame_count; frame_no++ ) {
    const Chunk this_frame_chunk = input( frame_no * frame_length, frame_length );
    XImage this_frame { this_frame_chunk, width, height };

    /* generate random barcode and add it to the frame */
    uint64_t barcode_num = uniform_distribution(generator);
    Barcode::writeBarcodes( this_frame, barcode_num );
    cerr << frame_no << "," << barcode_num << "\n";
    
    /* print out the image */
    stdout.write( this_frame.chunk() );
  }
  
  return EXIT_SUCCESS;
}
