#include "libcplot.hpp"
#include "toojpeg.h"

BitMap::BitMap(int width, int height) : width(width), height(height)
{
    pixels = new unsigned char[3 * width * height];
}

BitMap::~BitMap()
{
    delete[] pixels;
}

void BitMap::plot_complex_func(std::string expr, int maxval, bool grid, unsigned int nthreads)
{
    Parsing::Expression<std::complex<double>> func(expr);
    BitMap::plot_complex<double>(func, maxval, grid, nthreads);
}

void BitMap::save_jpeg(std::string filename)//Does what it says. .jpg extension not necessary
{
    filename += (filename.length() > 4 && filename.substr(filename.length() - 4, 4) == ".jpg" ? "" : ".jpg");

    jpeg_out = fopen(filename.c_str(), "wb");

    //temporary due to issues with _RGBpix obj and jpeg storage
    unsigned char* temp_pix = pixels;

    TooJpeg::writeJpeg([](unsigned char byte){fputc(byte, jpeg_out);},temp_pix, width, height, true, 100, false, NULL);

    fclose(jpeg_out);
    
}