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
        // handle error
        in_filename = "NO_FILE_PROVIDED";
        out_filename = "NO_FILE_PROVIDED";
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
            Barcoder::writeBarcodes(image, 8);

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
