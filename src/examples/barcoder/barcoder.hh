#include <iostream>
#include <fstream>
#include <array>
#include "display.hh"

static RGBPixel White = {0xFF, 0xFF, 0xFF, 0x0};
static RGBPixel Black = {0x0, 0x0, 0x0, 0x0};

class Barcoder {
public:
	Barcoder(std::ifstream& rawfile, int width, int height, int codesize, int nbitsPerRow) : 
		m_height(height), 
		m_width(width),
		m_bytes_per_row(width * 4),
		m_total_bits(nbitsPerRow * nbitsPerRow),
		m_code_size(codesize),
		m_rowBits(nbitsPerRow),
		m_next_frameno(0),
		m_input(rawfile)
	{}

	Barcoder(std::ifstream& rawfile) : 		
		m_height(720), 
		m_width(1280),
		m_bytes_per_row(1280 * 4),
		m_total_bits(4 * 4),
		m_code_size(16),
		m_rowBits(4),
		m_next_frameno(0),
		m_input(rawfile)
	{}

	~Barcoder() {}

	void writeBit(RGBPixel* frame_bytes, int x, int y, bool set);
	void barcodeFrame(RGBPixel *frame_bytes);
  	friend std::ostream& operator<<(std::ostream& os, Barcoder& br);

private:
	int m_height;
	int m_width;
	int m_bytes_per_row;
	int m_total_bits;
	int m_code_size;
	int m_rowBits;

	uint64_t m_next_frameno;
	std::ifstream& m_input;


};

std::ostream& operator<<(std::ostream& os, Barcoder& br)   {
	uint64_t frame_size = br.m_height * br.m_bytes_per_row;
	char* buf = new char[frame_size];
	while (br.m_input) {
		br.m_input.read(buf, frame_size);
		br.barcodeFrame((RGBPixel*)buf);
		os.write(buf, frame_size);
	}
	delete buf;
	return os;
}