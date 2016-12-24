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
    Scanner(int width, int height, int codesize, int nbitsPerRow) : 
        m_height(height), 
        m_width(width),
        m_bytes_per_row(width * 4),
        m_total_bits(nbitsPerRow * nbitsPerRow),
        m_code_size(codesize),
        m_rowBits(nbitsPerRow),
        m_next_frameno(0),
        m_logtimes(),
        m_logfile("scan.log")
    {   }

    Scanner() :        
        m_height(720), 
        m_width(1280),
        m_bytes_per_row(1280 * 4),
        m_total_bits(4 * 4),
        m_code_size(16),
        m_rowBits(4),
        m_next_frameno(0),
        m_logtimes(),
        m_logfile("scan.log")
    {   }

    ~Scanner() {
        m_logfile.close();
    }

    uint64_t scanUpperLeft(RGBPixel *frame_bytes) {
        uint64_t frameno = 0;
        for (int i = 0; i < m_total_bits; i++) {
            if (readBit(frame_bytes, 
                        (i % m_rowBits) * m_code_size, 
                        (i / m_rowBits) * m_code_size)) {
                frameno |= 1 << i;
            }
        }
        return frameno;
    }
    
    uint64_t scanLowerRight(RGBPixel *frame_bytes) {
        uint64_t frameno = 0;
        for (int i = 0; i < m_total_bits; i++) {
            if (readBit(frame_bytes, 
                        (m_width - (m_rowBits * m_code_size)) + (i % m_rowBits) * m_code_size, 
                        (m_height - (m_rowBits * m_code_size)) + (i / m_rowBits) * m_code_size)) {
                frameno |= 1 << i;
            }
        }
        return frameno;
    }

    void scanFrame(RGBPixel *frame_bytes) {
        uint64_t ul_frameno = scanUpperLeft(frame_bytes),
                 rl_frameno = scanLowerRight(frame_bytes);
        
        if (ul_frameno >= 0xFFFF || rl_frameno >= 0xFFFF) 
            return;

        //std::lock_guard<std::mutex> lg(tracker_mutex);
        time_point<high_resolution_clock> tp = high_resolution_clock::now();

        m_logfile   << m_next_frameno << " " << ul_frameno << " " << rl_frameno << " " 
                    << time_point_cast<microseconds>(tp).time_since_epoch().count() << std::endl;
        std::cout << "Scanned frame #" << m_next_frameno << " at " 
                  << time_point_cast<microseconds>(tp).time_since_epoch().count() 
                  << " with UL: " << ul_frameno << " RL: " << rl_frameno << std::endl;
    }
private:
    int m_height;
    int m_width;
    int m_bytes_per_row;
    int m_total_bits;
    int m_code_size;
    int m_rowBits;

    uint64_t m_next_frameno;
    std::vector<std::pair<uint64_t, time_point<high_resolution_clock> > > m_logtimes;
    std::ofstream m_logfile;

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

};

#endif
