/**
 * @file SobelFilter.cpp
 * @brief Implementation of the SobelFilter class
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

#include "SobelFilter.h"


/**
 * @brief Constructor for the SobelFilter class
 * 
 * Initializes a ConvolutionFilter with the name "Sobel" and a fixed kernel size of 3.
 */
SobelFilter::SobelFilter() : ConvolutionFilter("Sobel", KERNEL_SIZE) {
    // Additional initialization if needed
}

/**
 * @brief Apply the Sobel edge detection filter to an input image
 * 
 * This method first converts the image to greyscale (if it's RGB),
 * then applies the Sobel operator to detect horizontal and vertical edges,
 * and normalizes the result to produce an edge-detected greyscale image.
 * 
 * @param input The input image to be processed
 * @return A new Image object containing the Sobel edge detection result (greyscale)
 */
Image SobelFilter::apply(const Image& input) {
    // Convert to greyscale if needed
    Image greyscaleImage = input;
    if (input.getChannels() > 1) {
        greyscaleImage = input.toGreyscale();
    }
    
    int width = greyscaleImage.getWidth();
    int height = greyscaleImage.getHeight();
    Image output(width, height, 1);  // Output is a greyscale edge image

    int maxGradient = 0;
    std::vector<std::vector<int>> gradientMagnitude(height, std::vector<int>(width, 0));

    // Calculate horizontal (gradientX) and vertical (gradientY) gradients using Sobel kernels
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int gradientX = 0;
            int gradientY = 0;
            for (int ky = -1; ky <= 1; ++ky) {
                for (int kx = -1; kx <= 1; ++kx) {
                    int nx = std::max(0, std::min(width - 1, x + kx));
                    int ny = std::max(0, std::min(height - 1, y + ky));
                    unsigned char pixelValue = greyscaleImage.getPixel(nx, ny).getR();
                    gradientX += pixelValue * kernelX[ky + 1][kx + 1];
                    gradientY += pixelValue * kernelY[ky + 1][kx + 1];
                }
            }
            
            // Approximate magnitude: |G| = |Gx| + |Gy|
            int magnitude = std::abs(gradientX) + std::abs(gradientY);
            gradientMagnitude[y][x] = magnitude;
            if (magnitude > maxGradient) {
                maxGradient = magnitude;
            }
        }
    }
    
    // Normalize gradient values to [0, 255]
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            unsigned char normalizedValue = 0;
            if (maxGradient > 0) {
                normalizedValue = static_cast<unsigned char>(
                    (gradientMagnitude[y][x] * 255) / maxGradient
                );
            }
            Pixel edgePixel(normalizedValue, normalizedValue, normalizedValue);
            output.setPixel(x, y, edgePixel);
        }
    }
    
    return output;
}
