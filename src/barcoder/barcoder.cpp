#include "barcoder.hh"

static RGBPixel White = {0xFF, 0xFF, 0xFF, 0x0};
static RGBPixel Black = {0x0, 0x0, 0x0, 0x0};
static int barcode_grid_size = 4; // elements in each row and column
static int barcode_edge_len = 10; // height and width of each element (in pixels)

void Barcoder::applyBarcode(XImage& image, uint16_t barcode_num)
{
    // apply upper left (UL) barcode
    applyBarcode(image, barcode_num,
                 0,
                 0);

    // apply lower right (LR) barcode
    applyBarcode(image, barcode_num,
                 image.width() - barcode_grid_size*barcode_edge_len,
                 image.height() - barcode_grid_size*barcode_edge_len);
}

void Barcoder::applyBarcode(XImage& image, uint16_t barcode_num, int xpos, int ypos){
    unsigned int width = image.width();
    RGBPixel* raw_image = (RGBPixel*) image.data_unsafe();

    for (int i = 0; i < barcode_grid_size; i++) {
        for (int j = 0; j < barcode_grid_size; j++) {            
            int x_offset = barcode_edge_len * i + xpos;
            int y_offset = barcode_edge_len * j + ypos;
                
            for (int y = y_offset; y < y_offset + barcode_edge_len; y++) { 
                for (int x = x_offset; x < x_offset + barcode_edge_len; x++) {
                    bool pixel_set = barcode_num & (1 << (j*barcode_grid_size + i));
                    raw_image[y * width + x] = pixel_set ? Black : White;
                }
            }

        }
    }
}

uint16_t Barcoder::readBarcode(XImage& image){
    image.data();
    return 42;
}

uint16_t Barcoder::readBarcode(XImage& image, int x_offset, int y_offset){
    x_offset += 1;
    y_offset += 1;
    image.data();
    return 42;
}
