/**
 * @file testSharpeningFilter.cpp
 * @brief Tests for the SharpeningFilter class functionality
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

#include "TestCounters.h"      // Include the test counters and assertion macros
#include "../src/filter2D/SharpeningFilter.h"
#include "../src/Image.h"
#include "../src/Pixel.h"
#include <iostream>

/**
 * @brief Runs tests for the SharpeningFilter class
 *
 * Tests include:
 * - Verifying the constructor (optionally checking the filter name via getName())
 * - Applying the filter to a simple test image and verifying output dimensions and channel count
 * - Ensuring that the dark (black) area remains dark and the bright (white) area stays bright after filtering
 */
void runSharpeningFilterTests() {
    std::cout << "  Testing SharpeningFilter..." << std::endl;
    
    // Test 1: Constructor
    SharpeningFilter filter;
    // If the base Filter class provides a getName() method, you can check that the name is set correctly.
    // Example (uncomment if applicable):
    // CHECK(filter.getName() == "Sharpening", "SharpeningFilter constructor should set the correct name");
    
    // Test 2: Apply filter to a simple image
    // Create a 10x10 test image with 4 channels (RGBA)
    const int width = 10;
    const int height = 10;
    const int channels = 4;
    Image image(width, height, channels);
    
    // Initialize the entire image with white color
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            image.setPixel(x, y, Pixel(255, 255, 255, 255));
        }
    }
    
    // Draw a black square in the center (from (3,3) to (6,6))
    for (int y = 3; y < 7; y++) {
        for (int x = 3; x < 7; x++) {
            image.setPixel(x, y, Pixel(0, 0, 0, 255));
        }
    }
    
    // Check that calling apply does not throw an exception
    CHECK_NOTHROW(filter.apply(image), "SharpeningFilter::apply should not throw exceptions");
    
    // Apply the sharpening filter to the image
    Image result = filter.apply(image);
    
    // Verify that the output image dimensions and channel count match the input image
    CHECK(result.getWidth() == width, "Output image width should match input");
    CHECK(result.getHeight() == height, "Output image height should match input");
    CHECK(result.getChannels() == channels, "Output image channels should match input");
    
    // Verify that the dark area (e.g., center of the black square) remains relatively dark after filtering
    Pixel resultPixelCenter = result.getPixel(5, 5);
    CHECK(resultPixelCenter.getR() < 128, "After sharpening, the dark area should remain dark");
    
    // Verify that the white area (e.g., a corner pixel) remains relatively bright after filtering
    Pixel resultPixelCorner = result.getPixel(1, 1);
    CHECK(resultPixelCorner.getR() > 200, "After sharpening, the white area should remain bright");
    
    std::cout << "  SharpeningFilter tests completed." << std::endl;
}
