/**
 * @file ScharrFilter.cpp
 * @brief Implementation of the ScharrFilter class
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

#include "ScharrFilter.h"
#include <algorithm>
#include <cmath>
#include <vector>

/**
 * @brief Constructor for the ScharrFilter class
 */
ScharrFilter::ScharrFilter()
    : ConvolutionFilter("Scharr", KERNEL_SIZE) {
    // Additional initialization if needed
}

/**
 * @brief Apply the Scharr edge detection filter to an input image
 * @param input Input image to apply edge detection to
 * @return Edge-detected image after applying the filter
 */
Image ScharrFilter::apply(const Image& input) {
    // Convert to grayscale if needed
    Image grayscale = input;
    if (input.getChannels() > 1) {
        grayscale = input.toGreyscale();
    }

    int width = grayscale.getWidth();
    int height = grayscale.getHeight();
    Image output(width, height, 1); // Resulting edge image (grayscale)

    int maxGradient = 0;
    std::vector<std::vector<int>> gradientMagnitude(height, std::vector<int>(width, 0));

    // Compute horizontal (Gx) and vertical (Gy) gradients
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int gx = 0;
            int gy = 0;

            // Convolution with 3x3 kernels
            for (int ky = -1; ky <= 1; ++ky) {
                for (int kx = -1; kx <= 1; ++kx) {
                    int nx = std::max(0, std::min(width - 1, x + kx));
                    int ny = std::max(0, std::min(height - 1, y + ky));
                    unsigned char pixelValue = grayscale.getPixel(nx, ny).getR();

                    gx += pixelValue * kernelX[ky + 1][kx + 1];
                    gy += pixelValue * kernelY[ky + 1][kx + 1];
                }
            }

            // Approximate gradient magnitude
            int magnitude = std::abs(gx) + std::abs(gy);
            gradientMagnitude[y][x] = magnitude;
            if (magnitude > maxGradient) {
                maxGradient = magnitude;
            }
        }
    }

    // Normalize gradient to [0, 255]
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            unsigned char val = 0;
            if (maxGradient > 0) {
                val = static_cast<unsigned char>((gradientMagnitude[y][x] * 255) / maxGradient);
            }
            Pixel p(val, val, val);
            output.setPixel(x, y, p);
        }
    }

    return output;
}
