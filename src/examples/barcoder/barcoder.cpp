#include "barcoder.hh"

static RGBPixel White = {0xFF, 0xFF, 0xFF, 0x0};
static RGBPixel Black = {0x0, 0x0, 0x0, 0x0};

Barcoder::Barcoder(std::ifstream& rawfile, 
                    int width, int height, 
                    int codesize, int nbitsPerRow) : 
    m_height(height), 
    m_width(width),
    m_bytes_per_row(width * 4),
    m_total_bits(nbitsPerRow * nbitsPerRow),
    m_code_size(codesize),
    m_rowBits(nbitsPerRow),
    m_next_frameno(0),
    m_input(rawfile)
{}

Barcoder::Barcoder(std::ifstream& rawfile) :
    m_height(720), 
    m_width(1280),
    m_bytes_per_row(1280 * 4),
    m_total_bits(4 * 4),
    m_code_size(16),
    m_rowBits(4),
    m_next_frameno(0),
    m_input(rawfile)
{}

Barcoder::~Barcoder() {}

void Barcoder::barcodeFrameUpper(RGBPixel *frame_bytes, uint64_t frameno) {
    for (int i = 0; i < m_total_bits; i++) 
        writeBit(frame_bytes, 
                 (i % m_rowBits) * m_code_size, 
                 (i / m_rowBits) * m_code_size, 
                 frameno & (1 << i));
}
void Barcoder::barcodeFrameLower(RGBPixel *frame_bytes, uint64_t frameno) {
    for (int i = 0; i < m_total_bits; i++) 
        writeBit(frame_bytes, 
                 (m_width - (m_code_size * m_rowBits)) + (i % m_rowBits) * m_code_size, 
                 (m_height - (m_code_size * m_rowBits)) + (i / m_rowBits) * m_code_size, 
                 frameno & (1 << i));
}

void Barcoder::barcodeFrame(RGBPixel *frame_bytes) {
    barcodeFrameUpper(frame_bytes, m_next_frameno);
    barcodeFrameLower(frame_bytes, m_next_frameno);
    m_next_frameno++;
}

void Barcoder::writeBit(RGBPixel* frame_bytes, int x, int y, bool set) {
    for (int i = y; i < y + m_code_size; i++) 
        for (int j = x; j < x + m_code_size; j++)
            frame_bytes[i * m_width + j] = set ? Black : White;

}

std::ostream& operator<<(std::ostream& os, Barcoder& br)   {
    const uint64_t frame_size = br.m_height * br.m_bytes_per_row;
    char buf[frame_size];
    while (true) {
        if (br.m_input.read(buf, frame_size).eof())
            break;
        br.barcodeFrame((RGBPixel*)buf);
        os.write(buf, frame_size);
    }
    return os;
}
