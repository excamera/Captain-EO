#include <iostream>
#include <fstream>
#include "barcoder.hh"
#include "display.hh"

int main(int argc, char **argv) {
    const char * in_filename;
    const char * out_filename;

    if (argc >= 3) {
        in_filename = argv[1];
        out_filename = argv[2];
    }
    else {
        std::cout << "usage: " << argv[0] << " <input> <output>" << std::endl;
        return -1;
    }
        
    std::ifstream infile;
    std::ofstream outfile;
    try { 
        infile.open(in_filename, std::ios::in|std::ios::binary);
        if (infile.fail()) 
            throw in_filename;
        
        outfile.open(out_filename, std::ios::out|std::ios::binary);
        if (outfile.fail()) 
            throw out_filename;

        int height = 720;
        int width = 1280;
        int size = height * width;

        while (!infile.eof()) {
            std::vector<RGBPixel> raw_image(size);
            RGBPixel pixel; 
            for (int i = 0; i < size; i++){
                infile.read ((char*)&pixel, sizeof(RGBPixel));
                raw_image[i] = pixel;
            }

            XImage image(raw_image, width, height);

            int barcode_num = 8;
            Barcoder::writeBarcodes(image, barcode_num);
            std::cout << "write (UL, LR) (" << barcode_num << ", " << barcode_num << ")" << std::endl;

            auto x = Barcoder::readBarcodes(image);
            std::cout << "read  (UL, LR) (" << x.first << ", " << x.second << ")" << std::endl;
            
            outfile.write((char*)image.data(), sizeof(RGBPixel)*height*width);
        }
    } catch (const char* filename) {
        std::cerr << "File " << filename << " could not be opened. Exiting." << std::endl;
        exit(1);
    } catch (std::exception &e) {}

    infile.close();
    outfile.close();
    return 0;
}
