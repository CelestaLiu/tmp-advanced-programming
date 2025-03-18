/**
 * @file SharpeningFilter.cpp
 * @brief Implementation of the SharpeningFilter class
 * @group [Euler]
 * 
 * Group members:
 * - [Davide Baino] ([esemsc-db24])
 * - [Qi Gao] ([esemsc-qg124])
 * - [Daniel Kaupa] ([esemsc-dbk24])
 * - [Leyao Liu] ([esemsc-ll1524])
 * - [Jiayi Lu] ([esemsc-jl7324])
 * - [Ananya Sinha] ([esemsc-as10524])
 * - [Mingwei Yan] ([esemsc-my324])
 */

#include "SharpeningFilter.h"
#include "Image.h"
#include "Pixel.h"
#include <algorithm>
#include <cmath>

SharpeningFilter::SharpeningFilter() : Filter("Sharpening") {
    // Initialize with default parameters if needed
}
 
Image SharpeningFilter::apply(const Image& input) {
    // Create a new image with the same dimensions and channels
    int width = input.getWidth();
    int height = input.getHeight();
    int channels = input.getChannels();
     
    Image output(width, height, channels);
     
    // Apply the convolution with Laplacian kernel
    for (int y = 0; y < height; ++y) {

        for (int x = 0; x < width; ++x) {
            // Get the original pixel
            Pixel originalPixel = input.getPixel(x, y);
            
            // Apply the Laplacian kernel for edge detection
            int sumR = 0, sumG = 0, sumB = 0;
            
            for (int ky = -1; ky <= 1; ++ky) {
                for (int kx = -1; kx <= 1; ++kx) {
                    // Calculate neighbor coordinates with boundary checking
                    int nx = std::max(0, std::min(width - 1, x + kx));
                    int ny = std::max(0, std::min(height - 1, y + ky));
                    
                    // Get the neighbor pixel

                    Pixel neighborPixel = input.getPixel(nx, ny);
                    
                    // Apply kernel weights
                    int kernelValue = kernel[ky + 1][kx + 1];
                    sumR += neighborPixel.getR() * kernelValue;
                    sumG += neighborPixel.getG() * kernelValue;
                    sumB += neighborPixel.getB() * kernelValue;
                }
            }
             
            // Calculate sharpened values by adding the Laplacian result to the original pixel
            // Isharp = Ioriginal + G
            int newR = originalPixel.getR() + sumR;
            int newG = originalPixel.getG() + sumG;
            int newB = originalPixel.getB() + sumB;
             
            // Clamp values to valid range [0, 255]
            newR = std::max(0, std::min(255, newR));
            newG = std::max(0, std::min(255, newG));
            newB = std::max(0, std::min(255, newB));
             
            // Create and set the sharpened pixel
            Pixel sharpenedPixel(
                static_cast<unsigned char>(newR),
                static_cast<unsigned char>(newG),
                static_cast<unsigned char>(newB),
                originalPixel.getA()  // Preserve original alpha
            );
             
            output.setPixel(x, y, sharpenedPixel);
        }
    }
     
    return output;
}