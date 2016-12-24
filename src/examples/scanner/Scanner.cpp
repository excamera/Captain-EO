#include "Scanner.hh"

int main() {
    std::ifstream infile("video-input.raw", std::ios::in|std::ios::binary);
    uint64_t frame_size = 1280 * 720 * 4;
    Scanner sc;
    char* buf = new char[frame_size];
    while (infile) {
        infile.read(buf, frame_size);
        sc.scanFrame((RGBPixel*)buf);
    }
    delete buf;
    infile.close();
    return 0;
}
