#include "barcoder.hh"

void Barcoder::barcodeFrame(RGBPixel *frame_bytes) {
  	for (int i = 0; i < m_total_bits; i++) 
  		writeBit(frame_bytes, 
  				 (i % m_rowBits) * m_code_size, 
  				 (i / m_rowBits) * m_code_size, 
  				 m_next_frameno & (1 << i));

  	m_next_frameno++;
}

void Barcoder::writeBit(RGBPixel* frame_bytes, int x, int y, bool set) {
	for (int i = y; i < y + m_code_size; i++) 
		for (int j = x; j < x + m_code_size; j++)
			frame_bytes[i * m_width + j] = set ? Black : White;

}

int main() {
	std::ifstream infile("input-video.raw", std::ios::in|std::ios::binary);
	std::ofstream outfile("barcoded-video.raw", std::ios::out|std::ios::binary);
	Barcoder br(infile);
	outfile << br;
	infile.close();
	outfile.close();
	return 0;
}