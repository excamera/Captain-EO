#include "barcoder.hh"

static RGBPixel White = {0xFF, 0xFF, 0xFF, 0x0};
static RGBPixel Black = {0x0, 0x0, 0x0, 0x0};
static int code_rows = 4; // bits per column of code
static int code_cols = code_rows;
static int code_width = 20; // code width in pixels
static int code_height = code_width;

void Barcoder::applyBarcode(XImage& image, uint16_t barcode_num){
    barcode_num += 1;
    image.data();
}

void Barcoder::applyBarcode(XImage& image, uint16_t barcode_num, int x_offset, int y_offset){
    unsigned int width = image.width();
    RGBPixel* raw_image = (RGBPixel*) image.data_unsafe();

    for (int i = 0; i < code_rows; i++) {
        for (int j = 0; j < code_cols; j++) {            

            for (int y = y_offset; y < y_offset + code_height; y++) { 
                for (int x = x_offset; x < x_offset + code_width; x++) {
                    bool pixel_set = barcode_num & (1 << (i*code_rows + j));
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

// Barcoder::Barcoder(std::ifstream& rawfile, 
//                     int width, int height, 
//                     int codesize, int nbitsPerRow) : 
//     m_height(height), 
//     m_width(width),
//     m_bytes_per_row(width * 4),
//     m_total_bits(nbitsPerRow * nbitsPerRow),
//     m_code_size(codesize),
//     m_rowBits(nbitsPerRow),
//     m_next_frameno(0),
//     m_input(rawfile)
// {}

// Barcoder::Barcoder(std::ifstream& rawfile) :
//     m_height(720), 
//     m_width(1280),
//     m_bytes_per_row(1280 * 4),
//     m_total_bits(4 * 4),
//     m_code_size(16),
//     m_rowBits(4),
//     m_next_frameno(0),
//     m_input(rawfile)
// {}

// Barcoder::~Barcoder() {}

// void Barcoder::barcodeFrameUpper(RGBPixel *frame_bytes, uint64_t frameno) {
//     for (int i = 0; i < m_total_bits; i++) 
//         writeBit(frame_bytes, 
//                  (i % m_rowBits) * m_code_size, 
//                  (i / m_rowBits) * m_code_size, 
//                  frameno & (1 << i));
// }
// void Barcoder::barcodeFrameLower(RGBPixel *frame_bytes, uint64_t frameno) {
//     for (int i = 0; i < m_total_bits; i++) 
//         writeBit(frame_bytes, 
//                  (m_width - (m_code_size * m_rowBits)) + (i % m_rowBits) * m_code_size, 
//                  (m_height - (m_code_size * m_rowBits)) + (i / m_rowBits) * m_code_size, 
//                  frameno & (1 << i));
// }

// void Barcoder::barcodeFrame(RGBPixel *frame_bytes) {
//     barcodeFrameUpper(frame_bytes, m_next_frameno);
//     barcodeFrameLower(frame_bytes, m_next_frameno);
//     m_next_frameno++;
// }

// void Barcoder::writeBit(RGBPixel* frame_bytes, int x, int y, bool set) {
//     for (int i = y; i < y + code_width; i++) 
//         for (int j = x; j < x + code_height; j++)
//             frame_bytes[i * m_width + j] = set ? Black : White;
// }

// std::ostream& operator<<(std::ostream& os, Barcoder& br)   {
//     const uint64_t frame_size = br.m_height * br.m_bytes_per_row;
//     std::vector<char> buf_vector( frame_size );
//     char *buf = buf_vector.data();
//     while (true) {
//         if (br.m_input.read(buf, frame_size).eof())
//             break;
//         br.barcodeFrame((RGBPixel*)buf);
//         os.write(buf, frame_size);
//     }
//     return os;
// }
