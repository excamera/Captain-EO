#include "barcoder.hh"

static const char* infile_default = "video.raw";
static const char* outfile_default = "barcoded-video.raw";

int main(int argc, char **argv) {
    const char * in_filename = infile_default, *out_filename = outfile_default;
    if (argc >= 2) 
        in_filename = argv[1];
    if (argc >= 3)
        out_filename = argv[2];
    
    std::ifstream infile;
    std::ofstream outfile;
    try { 
        infile.open(in_filename, std::ios::in|std::ios::binary);
        if (infile.fail()) 
            throw in_filename;
        
        outfile.open(out_filename, std::ios::out|std::ios::binary);
        if (outfile.fail()) 
            throw out_filename;
        
        Barcoder br(infile);
        outfile << br;
        
    } catch (const char* filename) {
        std::cerr << "File " << filename << " could not be opened. Exiting." << std::endl;
        exit(1);
    } catch (std::exception &e) {}

    infile.close();
    outfile.close();
    return 0;
}
