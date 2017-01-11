#include <cstdint>
#include "barcoder.hh"

static RGBPixel White = {0xFF, 0xFF, 0xFF, 0x0};
static RGBPixel Black = {0x0, 0x0, 0x0, 0x0};
static int barcode_grid_size = 4; // elements in each row and column
static int barcode_edge_len = 10; // height and width of each element (in pixels)

void Barcoder::writeBarcodes(XImage& image, uint16_t barcode_num)
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

void Barcoder::writeBarcodeToPos(XImage& image, uint16_t barcode_num, int xpos, int ypos)
{
    unsigned int width = image.width();
    RGBPixel* raw_image = (RGBPixel*) image.data_unsafe();

    for (int i = 0; i < barcode_grid_size; i++) {
        for (int j = 0; j < barcode_grid_size; j++) {            
            int x_offset = barcode_edge_len * i + xpos;
            int y_offset = barcode_edge_len * j + ypos;

            // draw barcode element
            for (int y = y_offset; y < y_offset + barcode_edge_len; y++) { 
                for (int x = x_offset; x < x_offset + barcode_edge_len; x++) {
                    bool pixel_set = barcode_num & (1 << (j*barcode_grid_size + i));
                    raw_image[y * width + x] = pixel_set ? Black : White;
                }
            }

        }
    }
}

std::pair<uint16_t, uint16_t> Barcoder::readBarcodes(XImage& image)
{
    // read upper left (UL) barcode
    uint16_t upper_left = readBarcodeFromPos(image,
                                             0,
                                             0);
    
    // read lower right (LR) barcode
    uint16_t lower_right = readBarcodeFromPos(image,
                                              image.width() - barcode_grid_size*barcode_edge_len,
                                              image.height() - barcode_grid_size*barcode_edge_len);
    return std::make_pair(upper_left, lower_right);
}

uint16_t Barcoder::readBarcodeFromPos(XImage& image, int xpos, int ypos)
{
    uint16_t frame_num = 0;
    unsigned int width = image.width();
    RGBPixel* raw_image = (RGBPixel*) image.data_unsafe();

    for (int i = 0; i < barcode_grid_size; i++) {
        for (int j = 0; j < barcode_grid_size; j++) {            
            int x_offset = barcode_edge_len * i + xpos;
            int y_offset = barcode_edge_len * j + ypos;

            // read average value of barcode element
            double average = 0;
            for (int y = y_offset; y < y_offset + barcode_edge_len; y++) { 
                for (int x = x_offset; x < x_offset + barcode_edge_len; x++) {
                    RGBPixel* p = &raw_image[y * width + x];
                    average += (p->blue + p->red + p->green) / 3.0;
                }
            }
            average /= (barcode_edge_len * barcode_edge_len);

            bool bit_set = average < (1 << (8*sizeof(((RGBPixel *)0)->blue) - 1));
            frame_num |= bit_set ? (1 << (j*barcode_grid_size + i)) : 0;
        }
    }

    return frame_num;
}
