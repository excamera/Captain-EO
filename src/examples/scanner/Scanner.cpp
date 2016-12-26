#include "Scanner.hh"

int main() {
    const char * filename = "barcoded-video.raw";
    std::ifstream infile(filename, std::ios::in|std::ios::binary);
    if (infile.fail()) {
        std::cerr << "File " << filename << " could not be opened. Exiting." << std::endl;
        exit(1);
    }
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
