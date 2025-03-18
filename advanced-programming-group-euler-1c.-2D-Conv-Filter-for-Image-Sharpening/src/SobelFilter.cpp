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
 #include <algorithm>
 #include <cmath>
 
 SobelFilter::SobelFilter() : Filter("Sobel") {
     // Initialize with default parameters if needed
 }
 
 Image SobelFilter::apply(const Image& input) {
     // Create a grayscale version of the input image if it's RGB
     Image grayscaleImage = input;
     if (input.getChannels() > 1) {
         grayscaleImage = input.toGrayscale();
     }
     
     int width = grayscaleImage.getWidth();
     int height = grayscaleImage.getHeight();
     
     // Create output image with the same dimensions
     Image output(width, height, 1);  // Edge image is always grayscale
     
     // Find maximum gradient value for normalization
     int maxGradient = 0;
     
     // Temporary array to store gradient magnitudes
     std::vector<std::vector<int>> gradientMagnitude(height, std::vector<int>(width, 0));
     
     // Apply Sobel operator
     for (int y = 0; y < height; ++y) {
         for (int x = 0; x < width; ++x) {
             int gradientX = 0;
             int gradientY = 0;
             
             // Apply convolution with Sobel kernels
             for (int ky = -1; ky <= 1; ++ky) {
                 for (int kx = -1; kx <= 1; ++kx) {
                     // Calculate neighbor coordinates with boundary checking
                     int nx = std::max(0, std::min(width - 1, x + kx));
                     int ny = std::max(0, std::min(height - 1, y + ky));
                     
                     // Get pixel intensity (since we're working with grayscale)
                     unsigned char pixelValue = grayscaleImage.getPixel(nx, ny).getR();
                     
                     // Apply kernel weights
                     gradientX += pixelValue * kernelX[ky + 1][kx + 1];
                     gradientY += pixelValue * kernelY[ky + 1][kx + 1];
                 }
             }
             
             // Calculate gradient magnitude using approximation: |G| = |Gx| + |Gy|
             // (faster than sqrt(Gx^2 + Gy^2) and often good enough for edge detection)
             int magnitude = std::abs(gradientX) + std::abs(gradientY);
             
             // Store magnitude for later normalization
             gradientMagnitude[y][x] = magnitude;
             
             // Track maximum value for normalization
             if (magnitude > maxGradient) {
                 maxGradient = magnitude;
             }
         }
     }
     
     // Normalize and set output pixels
     for (int y = 0; y < height; ++y) {
         for (int x = 0; x < width; ++x) {
             // Normalize to 0-255 range
             unsigned char normalizedValue = 0;
             if (maxGradient > 0) {  // Avoid division by zero
                 normalizedValue = static_cast<unsigned char>((gradientMagnitude[y][x] * 255) / maxGradient);
             }
             
             // Create and set the edge pixel
             Pixel edgePixel(normalizedValue, normalizedValue, normalizedValue);
             output.setPixel(x, y, edgePixel);
         }
     }
     
     return output;
 }