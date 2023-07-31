#ifndef LIBCPLOT_HPP
#define LIBCPLOT_HPP

#include <thread>
#include <cassert>
#include <vector>
#include <complex>

#include "expr_parsing_cpp/parsing.hpp"


namespace ComplexPlot
{
    template<typename T>
    static void cmplx_to_colour(unsigned char* pix, std::complex<T> num)
    {
        T mag = std::abs(num), arg = std::arg(num);
        mag = -5.0/(mag + 5) + 1;
        for(int i = 0; i < 3; ++i)
        {
            pix[i] = (unsigned char)(mag*(127.5*sin(arg + (M_PI * i) / 2) + 127.5));
        }
    }


};

/*
#pragma pack(push, 1) // Pack the struct tightly without padding

struct BMPHeader {
    // Bitmap File Header (14 bytes)
    const char signature[2] = {'B', 'M'};  // BM (2 bytes) - Bitmap file signature
    uint32_t fileSize;  // File size (4 bytes) - Total size of the BMP file in bytes
    uint16_t reserved1; // Reserved (2 bytes) - Must be set to 0
    uint16_t reserved2; // Reserved (2 bytes) - Must be set to 0
    uint32_t dataOffset; // Data offset (4 bytes) - Offset in bytes to the start of the image data

    // Bitmap Information Header (40 bytes)
    uint32_t headerSize;      // Header size (4 bytes) - Size of this header (40 bytes)
    int32_t  imageWidth;      // Image width (4 bytes) - Width of the image in pixels
    int32_t  imageHeight;     // Image height (4 bytes) - Height of the image in pixels (positive for bottom-up images, negative for top-down images)
    uint16_t planes;          // Color planes (2 bytes) - Number of color planes (must be set to 1)
    uint16_t bitDepth;        // Bits per pixel (2 bytes) - Number of bits per pixel (e.g., 1, 4, 8, 16, 24, 32)
    uint32_t compression;     // Compression method (4 bytes) - Compression method being used (0 for uncompressed)
    uint32_t imageSize;       // Image size (4 bytes) - Size of the raw image data (can be set to 0 for uncompressed images)
    int32_t  horizontalRes;   // Horizontal resolution (4 bytes) - Pixels per meter
    int32_t  verticalRes;     // Vertical resolution (4 bytes) - Pixels per meter
    uint32_t colorsUsed;      // Number of colors used (4 bytes) - Number of colors in the color palette (set to 0 for full-color images)
    uint32_t importantColors; // Number of important colors (4 bytes) - Number of important colors used (set to 0 to indicate all are important)
};

#pragma pack(pop) // Restore default struct alignment
*/


static FILE* jpeg_out;
class BitMap
{
    const int width, height;
    unsigned char* pixels;

    template<typename T>
    void plot_complex_sector
    (Parsing::Expression<std::complex<T>> expr, int start_row, int num_rows, int maxval, bool grid)
    {
        T x, y;
        double pixel_per_int = std::min(height, width) / (2.0 * maxval);

        for(int row = 0; row < num_rows && row + start_row < height; row++)
        {

            for(int j = 0; j < width; j++)
            {
                x = (j - width / 2) / pixel_per_int;
                y = (-(row + start_row) + height / 2) / pixel_per_int;

                if(grid && maxval <= 50 && (std::abs(y - std::floor(y)) < 0.002 || std::abs(x - std::floor(x)) < 0.002)) 
                    for(int i = 0; i < 3; ++i)
                    {
                        pixels[at_pos_index(row + start_row, j) + i] = 30;
                    }
                    
                else 
                    ComplexPlot::cmplx_to_colour(pixels + at_pos_index(row + start_row, j), expr.evaluate({{'z', {x, y}}}));
            }
        }
    }


    int at_pos_index(int row, int column)
    {
        assert(row < height && column < width);
        return 3 * (row * width + column);
    }

    template<typename T>
    void plot_complex
    (Parsing::Expression<std::complex<T>>& expr, int maxval, bool grid, unsigned int nthreads)
    {
        nthreads = (int)std::min(nthreads, std::thread::hardware_concurrency()); //no more threads than available processors
        int rows_per_thread = height / nthreads; //size of each horizontal slice

        std::cout << "Using " << nthreads << " threads...\n";
        std::vector<std::thread> threads;
        threads.reserve(nthreads + 1); 

        
        
        for(int row = 0; row < height - height % rows_per_thread; row += rows_per_thread)
        {
            threads.push_back(
                std::thread(
                    [=, this](){this->plot_complex_sector(expr, row, rows_per_thread, maxval, grid);}
                )
            );
        }

        if(height % nthreads)
        {
            threads.push_back(
                std::thread(
                    [=, this](){this->plot_complex_sector(expr, height - height % nthreads, height % nthreads, maxval, grid);}
                )
            );
        }
        
        for(auto& t : threads)
            t.join();
    }

    public:

        BitMap(int width, int height);

        ~BitMap();

        void plot_complex_func(std::string expr, int maxval, bool grid, unsigned int nthreads);

        void save_jpeg(std::string filename);
};

#endif