#include <iostream>
#include <fstream>
#include <array>
#include "display.hh"


class Barcoder {
public:
    Barcoder(std::ifstream& rawfile, int width, int height, int codesize, int nbitsPerRow);
    Barcoder(std::ifstream& rawfile);
    ~Barcoder(); 

    void barcodeFrame(RGBPixel *frame_bytes); 
    friend std::ostream& operator<<(std::ostream& os, Barcoder& br);

private:
    void barcodeFrameLower(RGBPixel *frame_bytes, uint64_t frameno);
    void barcodeFrameUpper(RGBPixel *frame_bytes, uint64_t frameno);
    void writeBit(RGBPixel* frame_bytes, int x, int y, bool set);

    int m_height;
    int m_width;
    int m_bytes_per_row;
    int m_total_bits;
    int m_code_size;
    int m_rowBits;

    uint64_t m_next_frameno;
    std::ifstream& m_input;
};

std::ostream& operator<<(std::ostream& os, Barcoder& br);
