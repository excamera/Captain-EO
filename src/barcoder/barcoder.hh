#pragma once

#include <iostream>
#include <fstream>
#include <array>
#include "display.hh"

class Barcoder {
public:
    // Barcoder(std::ifstream& rawfile, int width, int height, int codesize, int nbitsPerRow);
    // Barcoder(std::ifstream& rawfile);
    // ~Barcoder(); 

    static void applyBarcode(XImage& image, int barcode_num); 
    static int readBarcode(XImage& image);
    
    //void barcodeFrame(RGBPixel *frame_bytes); 
    // friend std::ostream& operator<<(std::ostream& os, Barcoder& br);

private:
    static void applyBarcode(XImage& image, int barcode_num, int x_offset, int y_offset); 
    static int readBarcode(XImage& image, int x_offset, int y_offset); 

    void writeBit(RGBPixel* frame_bytes, int x, int y, bool set);

    //void barcodeFrameLower(RGBPixel *frame_bytes, uint64_t frameno);
    //void barcodeFrameUpper(RGBPixel *frame_bytes, uint64_t frameno);

    // int m_height;
    // int m_width;
    // int m_bytes_per_row;
    // int m_total_bits;
    // int m_code_size;
    // int m_rowBits;

    // uint64_t m_next_frameno;
    // std::ifstream& m_input;
};

// std::ostream& operator<<(std::ostream& os, Barcoder& br);
