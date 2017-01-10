#ifndef __FRAME_TRACKER_H__
#define __FRAME_TRACKER_H__

#define IS_BIT_SET(num, nbits, idx) num & (1 << idx)

#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <bitset>
#include <mutex>
#include "display.hh"

using std::chrono::time_point;
using std::chrono::high_resolution_clock;
using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::time_point_cast;
using std::chrono::milliseconds;
using std::chrono::microseconds;

//std::mutex tracker_mutex;

class Scanner {
public:
    Scanner(int width, int height, int codesize, int nbitsPerRow);
    Scanner();

    ~Scanner();

    bool scanFrame(RGBPixel *frame_bytes);

private:
    uint64_t scanUpperLeft(RGBPixel *frame_bytes);    
    uint64_t scanLowerRight(RGBPixel *frame_bytes);
    bool readBit(RGBPixel* frame_bytes, int x, int y);
    
    int m_height;
    int m_width;
    int m_bytes_per_row;
    int m_total_bits;
    int m_code_size;
    int m_rowBits;

    uint64_t m_next_frameno;
    std::vector<std::pair<uint64_t, time_point<high_resolution_clock> > > m_logtimes;
    std::ofstream m_logfile;

};

#endif
