#pragma once

#include <cstdint>
#include "display.hh"

namespace Barcode {
    void writeBarcodes(RGBImage& image, uint64_t barcode_num);
    void writeBarcodeToPos(RGBImage& image, uint64_t barcode_num, const unsigned int xpos, const unsigned int ypos);

    std::pair<uint64_t, uint64_t> readBarcodes(const RGBImage& image);
    uint64_t readBarcodeFromPos(const RGBImage& image, const unsigned int xpos, const unsigned int ypos);

    /* reading barcodes from a UYVY Image. Writing is not supported */
    std::pair<uint64_t, uint64_t> readBarcodes(const UYVYImage& image);
    uint64_t readBarcodeFromPos(const UYVYImage& image, const unsigned int xpos, const unsigned int ypos);
}
