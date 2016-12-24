#ifndef __FRAME_TRACKER_H__
#define __FRAME_TRACKER_H__

#define IS_BIT_SET(num, nbits, idx) num & (1 << idx)

#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <bitset>
#include "display.hh"

static RGBPixel White = {0xFF, 0xFF, 0xFF, 0x0};
static RGBPixel Black = {0x0, 0x0, 0x0, 0x0};

using std::chrono::time_point;
using std::chrono::high_resolution_clock;
using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::time_point_cast;
using std::chrono::milliseconds;
using std::chrono::microseconds;

class FrameTracker {
public:
	FrameTracker(int width, int height, int codesize, int nbitsPerRow) : 
		m_height(height), 
		m_width(width),
		m_bytes_per_row(width * 4),
		m_total_bits(nbitsPerRow * nbitsPerRow),
		m_code_size(codesize),
		m_rowBits(nbitsPerRow),
		m_next_frameno(0),
		send_times(),
		recv_times(),
		sent_file("sent-pattern.txt"),
		recv_file("recv-pattern.txt")
	{	}

	FrameTracker() : 		
		m_height(720), 
		m_width(1280),
		m_bytes_per_row(1280 * 4),
		m_total_bits(4 * 4),
		m_code_size(16),
		m_rowBits(4),
		m_next_frameno(0),
		send_times(),
		recv_times(),
		sent_file("sent-pattern.txt"),
		recv_file("recv-pattern.txt")
	{	}

  	~FrameTracker() {
  		recv_file.close();
  		sent_file.close();
  	}


  	void markFrameAsSent(uint64_t frameno) {
  		auto tp = high_resolution_clock::now();

  		sent_file 	<< frameno << " "
  					<< time_point_cast<microseconds>(tp).time_since_epoch().count() << std::endl;

  		send_times.push_back(tp);
  	}

  	void registerFrame(RGBPixel *frame_bytes) {
  		uint64_t frameno = 0;
  		for (int i = 0; i < m_total_bits; i++) {
  			if (readBit(frame_bytes, 
  						(i % m_rowBits) * m_code_size, 
  						(i / m_rowBits) * m_code_size)) {
  				frameno |= 1 << i;
  			}
  		}
  			time_point<high_resolution_clock> tp = high_resolution_clock::now();
  			recv_times.emplace_back( frameno, tp );

  			recv_file 	<< frameno << " "
  						<< time_point_cast<microseconds>(tp).time_since_epoch().count() << std::endl;
  	}
private:
	int m_height;
	int m_width;
	int m_bytes_per_row;
	int m_total_bits;
	int m_code_size;
	int m_rowBits;

	uint64_t m_next_frameno;
	std::vector<time_point<high_resolution_clock> > send_times;
	std::vector<std::pair<uint64_t, time_point<high_resolution_clock> > > recv_times;
	std::ofstream sent_file;
	std::ofstream recv_file; 

	bool readBit(RGBPixel* frame_bytes, int x, int y) {
		double avg = 0;
		for (int i = y; i < y + m_code_size; i++) {
			for (int j = x; j < x + m_code_size; j++) {
				RGBPixel* p = &(frame_bytes[i * m_width + j]);
				avg += (p->blue + p->red + p->green) / 3.0;
			}
		}
		avg /= (m_code_size * m_code_size);
		//std::cerr << "Channel avg: " << avg << std::endl;
		return avg < 127;
	}

	void writeBit(RGBPixel* frame_bytes, int x, int y, bool set) {
		for (int i = y; i < y + m_code_size; i++) 
			for (int j = x; j < x + m_code_size; j++)
				frame_bytes[i * m_width + j] = set ? Black : White;

	}

};

#endif
