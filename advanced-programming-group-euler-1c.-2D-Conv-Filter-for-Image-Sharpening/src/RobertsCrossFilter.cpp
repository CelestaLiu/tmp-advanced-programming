/**
 * @file RobertsCrossFilter.cpp
 * @brief Implementation of the RobertsCrossFilter class
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

#include "RobertsCrossFilter.h"
#include <algorithm>
#include <cmath>
#include <vector>

/**
 * @brief Constructor for the RobertsCrossFilter class
 */
RobertsCrossFilter::RobertsCrossFilter()
    : ConvolutionFilter("RobertsCross", KERNEL_SIZE) {
    // Additional initialization if needed
}

/**
 * @brief Apply the Roberts Cross edge detection filter to an input image
 * @param input Input image to apply edge detection to
 * @return Edge-detected image after applying the filter
 */
Image RobertsCrossFilter::apply(const Image& input) {
    // Convert to grayscale if needed
    Image grayscale = input;
    if (input.getChannels() > 1) {
        grayscale = input.toGrayscale();
    }

    int width = grayscale.getWidth();
    int height = grayscale.getHeight();
    Image output(width, height, 1); // Resulting edge image (grayscale)

    int maxGradient = 0;
    std::vector<std::vector<int>> gradientMagnitude(height, std::vector<int>(width, 0));

    // For each pixel, apply the two 2x2 kernels
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int g1 = 0;
            int g2 = 0;

            for (int ky = 0; ky < 2; ++ky) {
                for (int kx = 0; kx < 2; ++kx) {
                    // Boundary check
                    int nx = std::max(0, std::min(width - 1, x + kx));
                    int ny = std::max(0, std::min(height - 1, y + ky));
                    unsigned char pixelValue = grayscale.getPixel(nx, ny).getR();

                    g1 += pixelValue * kernel1[ky][kx];
                    g2 += pixelValue * kernel2[ky][kx];
                }
            }

            // Approximate gradient magnitude: |G| = |G1| + |G2|
            int magnitude = std::abs(g1) + std::abs(g2);
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
