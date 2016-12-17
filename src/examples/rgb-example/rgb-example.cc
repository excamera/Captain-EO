#include <cstdlib>

#include "display.hh"

using namespace std;

int main()
{
  /* construct a window */
  XWindow window( 1280, 720 );
  window.set_name( "RGB example" );

  /* put the window on the screen */
  window.map();

  /* on the X server, construct a picture */
  XPixmap picture( window );

  /* in our program (the X client), construct an image */
  XImage image( picture );
  GraphicsContext gc( picture );

  /* draw alternating all-red or all-blue */
  bool red_or_blue = false;
  while ( true ) {
    for ( unsigned int col = 0; col < picture.size().first; col++ ) {
      for ( unsigned int row = 0; row < picture.size().second; row++ ) {
	image.pixel( col, row ).red =  red_or_blue ? 255 : 0;
	image.pixel( col, row ).blue = red_or_blue ? 0   : 255;
      }
    }

    /* paint the image (client-side) onto the picture (server-side) */
    picture.put( image, gc );

    /* tell the server to paint the picture onto the window */
    window.present( picture, 0, 0 );

    /* next time, paint a different color */
    red_or_blue = not red_or_blue;
  }

  return EXIT_SUCCESS;
}
