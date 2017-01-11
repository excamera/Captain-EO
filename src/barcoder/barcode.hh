#pragma once

#include <cstdint>
#include "display.hh"

namespace Barcode {
    void writeBarcodes(XImage& image, uint64_t barcode_num); 
    void writeBarcodeToPos(XImage& image, uint64_t barcode_num, const unsigned int xpos, const unsigned int ypos); 

    std::pair<uint64_t, uint64_t> readBarcodes(const XImage& image);
    uint64_t readBarcodeFromPos(const XImage& image, const unsigned int xpos, const unsigned int ypos); 
}
