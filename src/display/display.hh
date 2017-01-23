#ifndef DISPLAY_HH
#define DISPLAY_HH

#include <xcb/xcb.h>

#include <memory>
#include <vector>

#include "chunk.hh"

class XCBObject
{
public:
  typedef std::shared_ptr<xcb_connection_t> connection_type;
  XCBObject();
  XCBObject( XCBObject & original );
  XCBObject( XCBObject && original );
  virtual ~XCBObject() {}

  xcb_connection_t * xcb_connection() { return connection_.get(); }

private:
  connection_type connection_;

protected:
  void check_noreply( const std::string & context, const xcb_void_cookie_t & cookie );
  const xcb_screen_t * default_screen() const;
  const connection_type & connection() const { return connection_; }
};

class XPixmap;

class XWindow : public XCBObject
{
private:
  xcb_window_t window_ = xcb_generate_id( connection().get() );
  uint32_t complete_event_ = xcb_generate_id( connection().get() );
  uint32_t idle_event_ = xcb_generate_id( connection().get() );
  bool complete_ = true, idle_ = true;

  void event_loop();

public:
  XWindow( const unsigned int width, const unsigned int height );
  XWindow( const unsigned int width, const unsigned int height, const std::string &name);

  ~XWindow();

  /* set the name of the window */
  void set_name( const std::string & name );

  /* map the window on the screen */
  void map();

  /* present a pixmap */
  void present( const XPixmap & pixmap, const unsigned int divisor, const unsigned int remainder );

  /* flush XCB */
  void flush();

  /* get the window's size */
  std::pair<unsigned int, unsigned int> size() const;

  /* get the underlying window */
  const xcb_window_t & xcb_window() const { return window_; }

  /* get the window's visual */
  xcb_visualtype_t * xcb_visual();

  /* prevent copying */
  XWindow( const XWindow & other ) = delete;
  XWindow & operator=( const XWindow & other ) = delete;
};

class GraphicsContext : public XCBObject
{
private:
  xcb_gcontext_t gc_ = xcb_generate_id( connection().get() );

public:
  GraphicsContext( XPixmap & pixmap );

  /* get the underlying gc */
  const xcb_gcontext_t & xcb_gc() const { return gc_; }
};

template<class PixelType> class XImage;

class XPixmap : public XCBObject
{
private:
  xcb_pixmap_t pixmap_ = xcb_generate_id( connection().get() );
  xcb_visualtype_t * visual_;
  std::pair<unsigned int, unsigned int> size_;

public:
  XPixmap( XWindow & window );
  ~XPixmap();

  /* get the underlying pixmap */
  const xcb_pixmap_t & xcb_pixmap() const { return pixmap_; }

  /* get the pixmap's visual */
  xcb_visualtype_t * xcb_visual() const { return visual_; }

  /* get the pixmap's size */
  std::pair<unsigned int, unsigned int> size() const { return size_; }

  /* put an image on the pixmap */
  template<class PixelType>
  void put( const XImage<PixelType> & image, const GraphicsContext & gc );

  /* prevent copying */
  XPixmap( const XPixmap & other ) = delete;
  XPixmap & operator=( const XPixmap & other ) = delete;
};

struct RGBPixel
{
  uint8_t blue, green, red, xxx;
};

struct UYVYPixel
{
  uint8_t cbcr, y;
};

template<class PixelType>
class XImage
{
private:
  unsigned int width_, height_;
  std::vector<PixelType> image_;

public:
  XImage( XPixmap & pixmap );
  XImage( const Chunk & image, const unsigned int width, const unsigned int height );

  const PixelType & pixel( const unsigned int column, const unsigned int row ) const;
  PixelType & pixel( const unsigned int column, const unsigned int row );
  const uint8_t * data() const { return reinterpret_cast<const uint8_t*>( &image_.at( 0 ) ); }
  uint8_t * data_unsafe() { return reinterpret_cast<uint8_t*>( &image_.at( 0 ) ); };

  Chunk chunk() const { return Chunk( data(), image_.size() * sizeof( PixelType ) ); }

  unsigned int width() const { return width_; }
  unsigned int height() const { return height_; }
};

using RGBImage = XImage<RGBPixel>;
using UYVYImage = XImage<UYVYPixel>;

#endif
