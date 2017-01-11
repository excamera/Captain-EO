#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>

#include <xcb/present.h>

#include "display.hh"
#include "chunk.hh"

using namespace std;

template <typename T>
inline T * notnull( const string & context, T * const x )
{
  return x ? x : throw runtime_error( context + ": returned null pointer" );
}

struct free_deleter
{
  template <typename T>
  void operator() ( T * x ) const { free( x ); }
};

void XCBObject::check_noreply( const string & context, const xcb_void_cookie_t & cookie )
{
  unique_ptr<xcb_generic_error_t, free_deleter> error { xcb_request_check( connection_.get(), cookie ) };
  if ( error ) {
    throw runtime_error( context + ": returned error " + to_string( error->error_code ) );
  }
}

const xcb_screen_t * XCBObject::default_screen() const
{
  return notnull( "xcb_setup_roots_iterator",
		  xcb_setup_roots_iterator( notnull( "xcb_get_setup",
						     xcb_get_setup( connection_.get() ) ) ).data );  
}

XCBObject::XCBObject()
  : connection_( notnull( "xcb_connect", xcb_connect( nullptr, nullptr ) ),
		 [] ( xcb_connection_t * connection ) { xcb_disconnect( connection ); } )
{}

XCBObject::XCBObject( XCBObject & original )
  : connection_( original.connection() )
{}


XCBObject::XCBObject( XCBObject && original )
  : connection_( move( original.connection_ ) )
{}

XWindow::XWindow( const unsigned int width, const unsigned int height )
{
  const auto screen = default_screen();

  check_noreply( "xcb_create_window_checked",
		 xcb_create_window_checked( connection().get(),
					    XCB_COPY_FROM_PARENT,
					    window_,
					    screen->root,
					    0, 0, /* top-left corner of the window */
					    width, height,
					    0, /* border size */
					    XCB_WINDOW_CLASS_INPUT_OUTPUT,
					    screen->root_visual,
					    0, nullptr ) ); /* value_mask, value_list */

  check_noreply( "xcb_present_select_input_checked",
		 xcb_present_select_input_checked( connection().get(),
						   complete_event_,
						   window_,
						   XCB_PRESENT_EVENT_MASK_COMPLETE_NOTIFY ) );

  check_noreply( "xcb_present_select_input_checked",
		 xcb_present_select_input_checked( connection().get(),
						   idle_event_,
						   window_,
						   XCB_PRESENT_EVENT_MASK_IDLE_NOTIFY ) );
}

XWindow::XWindow( const unsigned int width, const unsigned int height, 
                  const std::string &name ) 
{
  const auto screen = default_screen();

  check_noreply( "xcb_create_window_checked",
     xcb_create_window_checked( connection().get(),
              XCB_COPY_FROM_PARENT,
              window_,
              screen->root,
              0, 0, /* top-left corner of the window */
              width, height,
              0, /* border size */
              XCB_WINDOW_CLASS_INPUT_OUTPUT,
              screen->root_visual,
              0, nullptr ) ); /* value_mask, value_list */

  check_noreply( "xcb_present_select_input_checked",
     xcb_present_select_input_checked( connection().get(),
               complete_event_,
               window_,
               XCB_PRESENT_EVENT_MASK_COMPLETE_NOTIFY ) );

  check_noreply( "xcb_present_select_input_checked",
     xcb_present_select_input_checked( connection().get(),
               idle_event_,
               window_,
               XCB_PRESENT_EVENT_MASK_IDLE_NOTIFY ) );

  set_name(name);
  map();
}


void XWindow::map() {
  check_noreply( "xcb_map_window_checked",
		 xcb_map_window_checked( connection().get(), window_ ) );
}

void XWindow::flush() {
  if ( xcb_flush( connection().get() ) <= 0 ) {
    throw runtime_error( "xcb_flush: failed" );
  }
}

XWindow::~XWindow()
{
  try {
    check_noreply( "xcb_destroy_window_checked",
		   xcb_destroy_window_checked( connection().get(), window_ ) );
  } catch ( const exception & e ) {
    cerr << e.what() << endl;
  }
}

void XWindow::set_name( const string & name )
{
  check_noreply( "xcb_change_property_checked",
		 xcb_change_property_checked( connection().get(),
					      XCB_PROP_MODE_REPLACE,
					      window_,
					      XCB_ATOM_WM_NAME,
					      XCB_ATOM_STRING,
					      8, /* 8-bit string */
					      name.length(),
					      name.data() ) );
}

xcb_visualtype_t * XWindow::xcb_visual()
{
  /* get the visual ID of the window */
  xcb_get_window_attributes_cookie_t cookie = xcb_get_window_attributes( connection().get(), window_ );
  unique_ptr<xcb_get_window_attributes_reply_t, free_deleter> attributes { notnull( "xcb_get_attributes_reply",
										    xcb_get_window_attributes_reply( connection().get(), cookie, nullptr ) ) };

  /* now find the corresponding visual */
  const auto screen = default_screen();

  for ( xcb_depth_iterator_t depth_it = xcb_screen_allowed_depths_iterator( screen );
	depth_it.rem;
	xcb_depth_next( &depth_it ) ) {
    for ( xcb_visualtype_iterator_t visualtype_it = xcb_depth_visuals_iterator( depth_it.data );
	  visualtype_it.rem;
	  xcb_visualtype_next( &visualtype_it ) ) {
      if ( attributes->visual == visualtype_it.data->visual_id ) {
	return visualtype_it.data;
      }
    }
  }

  throw runtime_error( "xcb_visual: no matching visualtype_t found" );
}

pair<unsigned int, unsigned int> XWindow::size() const
{
  xcb_get_geometry_cookie_t cookie = xcb_get_geometry( connection().get(), window_ );
  unique_ptr<xcb_get_geometry_reply_t, free_deleter> geometry { notnull( "xcb_get_geometry_reply",
									 xcb_get_geometry_reply( connection().get(), cookie, nullptr ) ) };
  return make_pair( geometry->width, geometry->height );
}

XPixmap::XPixmap( XWindow & window )
  : XCBObject( window ),
    visual_( window.xcb_visual() ),
    size_( window.size() )
{
  const uint8_t depth = default_screen()->root_depth;

  /* make sure we are in 24-bit color mode */
  if ( depth != 24 ) {
    throw runtime_error( string( "Needed 24-bit depth, but screen has " )
			 + to_string( depth )
			 + " instead" );
  }

  /* make sure the colors are where we expect them */
  if ( visual_->red_mask != 0xFF0000
       or visual_->green_mask != 0x00FF00
       or visual_->blue_mask != 0x0000FF ) {
    std::ostringstream color_layout;
    color_layout << "Unexpected color layout: ";
    color_layout << hex << "red=" << visual_->red_mask;
    color_layout << hex << ", green=" << visual_->green_mask;
    color_layout << hex << ", blue=" << visual_->blue_mask;
    throw runtime_error( color_layout.str() );
  }

  /* finally, make the pixmap */
  check_noreply( "xcb_create_pixmap_checked",
		 xcb_create_pixmap_checked( connection().get(),
					    depth,
					    pixmap_,
					    window.xcb_window(),
					    size_.first, size_.second ) );
}

XPixmap::~XPixmap()
{
  try {
    check_noreply( "xcb_free_pixmap_checked",
		   xcb_free_pixmap_checked( connection().get(), pixmap_ ) );
  } catch ( const exception & e ) {
    cerr << e.what() << endl;
  }
}

void XWindow::present( const XPixmap & pixmap, const unsigned int divisor, const unsigned int remainder )
{
  while ( not complete_ ) {
    event_loop();
  }

  check_noreply( "xcb_present_pixmap_checked",
		 xcb_present_pixmap_checked( connection().get(),
					     window_,
					     pixmap.xcb_pixmap(),
					     0, /* serial */
					     0, /* valid */
					     0, /* update */
					     0, 0, /* offsets */
					     0, /* target_crtc */
					     0, /* wait_fence */
					     0, /* idle_fence */
					     0, /* options */
					     0, /* target_msc */
					     divisor, /* divisor */
					     remainder, /* remainder */
					     0, /* notifies_len */
					     nullptr /* notifies */ ) );

  complete_ = false;
  idle_ = false;

  while ( not idle_ ) {
    event_loop();
  }
}

void XWindow::event_loop()
{
  xcb_generic_event_t * event = notnull( "xcb_wait_for_event",
					 xcb_wait_for_event( connection().get() ) );
  if ( event->response_type == XCB_GE_GENERIC ) {
    const uint16_t event_type = reinterpret_cast<xcb_ge_generic_event_t *>( event )->event_type;
    if ( event_type == (complete_event_ & 0xffff) ) {
      complete_ = true;
    } else if ( event_type == (idle_event_ & 0xffff) ) {
      idle_ = true;
    } else {
      throw runtime_error( "unexpected present event" );
    }
  } else {
    cerr << "Unexpected response of type " << int( event->response_type ) << "\n";
  }
}

XImage::XImage( XPixmap & pixmap )
  : width_( pixmap.size().first ),
    height_( pixmap.size().second ),
    image_( width_ * height_ )
{}

XImage::XImage( const Chunk & image, const unsigned int width, const unsigned int height )
  : width_( width ),
    height_( height ),
    image_()
{
  if ( image.size() != width * height * sizeof( RGBPixel ) ) {
    throw runtime_error( "XImage: invalid chunk size" );
  }

  image_.resize( width * height );
  memcpy( image_.data(), image.buffer(), image.size() );
}

void XPixmap::put( const XImage & image, const GraphicsContext & gc )
{
  check_noreply( "xcb_put_image_checked",
		 xcb_put_image_checked( connection().get(),
					XCB_IMAGE_FORMAT_Z_PIXMAP,
					xcb_pixmap(),
					gc.xcb_gc(),
					image.width(),
					image.height(),
					0,
					0,
					0,
					24,
					image.width() * image.height() * sizeof( RGBPixel ),
					image.data() ) );
}

const RGBPixel & XImage::pixel( const unsigned int column, const unsigned int row ) const
{
  if ( column >= width_ or row >= height_ ) {
    throw out_of_range( "attempted access to pixel outside image" );
  }

  return image_.at( row * width() + column );
}

RGBPixel & XImage::pixel( const unsigned int column, const unsigned int row ) 
{
  if ( column >= width_ or row >= height_ ) {
    throw out_of_range( "attempted access to pixel outside image" );
  }

  return image_.at( row * width() + column );
}

GraphicsContext::GraphicsContext( XPixmap & pixmap )
  : XCBObject( pixmap )
{
  check_noreply( "xcb_create_gc_checked",
		 xcb_create_gc_checked( connection().get(),
					gc_,
					pixmap.xcb_pixmap(),
					0,
					nullptr ) );
}
