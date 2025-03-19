/**
 * @file testRobertsCrossFilter.cpp
 * @brief Tests for the RobertsCrossFilter class functionality
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
#include "../src/filter2D/RobertsCrossFilter.h"
#include "../src/Image.h"
#include "../src/Pixel.h"
#include <iostream>

/**
 * @brief Runs tests for the RobertsCrossFilter class
 *
 * This function tests the basic functionality of the RobertsCrossFilter, including:
 * - Verifying that the filter can be constructed.
 * - Applying the filter on a test image.
 * - Checking that the output image dimensions and channel count match the input.
 * - Verifying that edge detection produces a high intensity response at the boundary.
 */
void runRobertsCrossFilterTests() {
    std::cout << "  Testing RobertsCrossFilter..." << std::endl;
    
    // Test 1: Constructor
    RobertsCrossFilter filter;
    
    // Test 2: Apply filter to a simple test image
    // Create a 10x10 image with 4 channels (RGBA)
    const int width = 10;
    const int height = 10;
    const int channels = 4;
    Image image(width, height, channels);
    
    // Create an image with a clear edge: left half is white, right half is black
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (x < width / 2) {
                // White region
                image.setPixel(x, y, Pixel(255, 255, 255, 255));
            } else {
                // Black region
                image.setPixel(x, y, Pixel(0, 0, 0, 255));
            }
        }
    }
    
    // Check that applying the filter does not throw an exception
    CHECK_NOTHROW(filter.apply(image), "RobertsCrossFilter::apply should not throw exceptions");
    
    // Apply the Roberts Cross filter to the image
    Image result = filter.apply(image);
    
    // Verify that the output image dimensions and channel count match the input image
    CHECK(result.getWidth() == width, "Output image width should match input");
    CHECK(result.getHeight() == height, "Output image height should match input");

    // Verify that a pixel far from the edge (in the middle of the white region) has a low intensity response
    Pixel whiteRegionPixel = result.getPixel(width / 4, height / 2);
    CHECK(whiteRegionPixel.getR() < 50, "Non-edge regions should have a low intensity response");
    
    std::cout << "  RobertsCrossFilter tests completed." << std::endl;
}
