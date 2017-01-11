#pragma once

#include <cstdint>
#include "display.hh"

namespace Barcoder {
    void writeBarcodes(XImage& image, uint64_t barcode_num); 
    void writeBarcodeToPos(XImage& image, uint64_t barcode_num, const unsigned int xpos, const unsigned int ypos); 

    std::pair<uint64_t, uint64_t> readBarcodes(XImage& image);
    uint64_t readBarcodeFromPos(XImage& image, const unsigned int xpos, const unsigned int ypos); 
}
