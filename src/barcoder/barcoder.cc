#include <cassert>
#include <cstdint>
#include <limits>
#include "barcoder.hh"

static RGBPixel White = {0xFF, 0xFF, 0xFF, 0x0};
static RGBPixel Black = {0x0, 0x0, 0x0, 0x0};
static unsigned int barcode_grid_size = 8; // elements in each row and column
static unsigned int barcode_edge_len = 10; // height and width of each element (in pixels)

void Barcoder::writeBarcodes(XImage& image, const uint64_t barcode_num)
{
    // write upper left (UL) barcode
    writeBarcodeToPos(image,
                      barcode_num,
                      0,
                      0);

    // write lower right (LR) barcode
    writeBarcodeToPos(image,
                      barcode_num,
                      image.width() - barcode_grid_size*barcode_edge_len,
                      image.height() - barcode_grid_size*barcode_edge_len);
}

void Barcoder::writeBarcodeToPos(XImage& image, const uint64_t barcode_num,
                                 const unsigned int xpos,
                                 const unsigned int ypos)
{
    for (unsigned int i = 0; i < barcode_grid_size; i++) {
        for (unsigned int j = 0; j < barcode_grid_size; j++) {            
            unsigned int x_offset = barcode_edge_len * i + xpos;
            unsigned int y_offset = barcode_edge_len * j + ypos;

            // draw barcode element
            for (unsigned int y = y_offset; y < y_offset + barcode_edge_len; y++) { 
                for (unsigned int x = x_offset; x < x_offset + barcode_edge_len; x++) {
                    bool pixel_set = barcode_num & (1 << (j*barcode_grid_size + i));
                    image.pixel(x,y) = pixel_set ? Black : White;
                }
            }

        }
    }
}

std::pair<uint64_t, uint64_t> Barcoder::readBarcodes(XImage& image)
{
    // read upper left (UL) barcode
    uint64_t upper_left = readBarcodeFromPos(image,
                                             0,
                                             0);
    
    // read lower right (LR) barcode
    uint64_t lower_right = readBarcodeFromPos(image,
                                              image.width() - barcode_grid_size*barcode_edge_len,
                                              image.height() - barcode_grid_size*barcode_edge_len);

    return std::make_pair(upper_left, lower_right);
}

uint64_t Barcoder::readBarcodeFromPos(XImage& image,
                                      const unsigned int xpos,
                                      const unsigned int ypos)
{
    uint64_t frame_num = 0;

    for (unsigned int i = 0; i < barcode_grid_size; i++) {
        for (unsigned int j = 0; j < barcode_grid_size; j++) {            
            unsigned int x_offset = barcode_edge_len * i + xpos;
            unsigned int y_offset = barcode_edge_len * j + ypos;
            
            // read average value of barcode element
            double average = 0;
            for (unsigned int y = y_offset; y < y_offset + barcode_edge_len; y++) { 
                for (unsigned int x = x_offset; x < x_offset + barcode_edge_len; x++) {
                    RGBPixel & p = image.pixel(x, y);
                    average += (p.blue + p.green + p.red) / 3.0;
                }
            }
            average /= (barcode_edge_len * barcode_edge_len);

            {
                RGBPixel p;
                assert(sizeof(uint8_t) == 1);
                assert(sizeof(p.blue) == sizeof(uint8_t));
                assert(sizeof(p.green) == sizeof(uint8_t));
                assert(sizeof(p.red) == sizeof(uint8_t));
            }

            bool bit_set = average < 128;
            frame_num |= bit_set ? (1 << (j*barcode_grid_size + i)) : 0;
        }
    }

    return frame_num;
}
