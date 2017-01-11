#pragma once

#include <cstdint>
#include "display.hh"

namespace Barcoder {
    void writeBarcodes(XImage& image, uint16_t barcode_num); 
    void writeBarcodeToPos(XImage& image, uint16_t barcode_num, int xpos, int ypos); 

    uint16_t readBarcodes(XImage& image);
    uint16_t readBarcodeFromPos(XImage& image, int xpos, int ypos); 
}
