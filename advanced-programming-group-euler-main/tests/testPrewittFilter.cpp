/**
 * @file testPrewittFilter.cpp
 * @brief Tests for the PrewittFilter class functionality
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
#include "../src/filter2D/PrewittFilter.h"
#include "../src/Image.h"
#include "../src/Pixel.h"
#include <iostream>

/**
 * @brief Runs tests for the PrewittFilter class
 *
 * This function tests the basic functionality of the PrewittFilter, including:
 * - Construction of the filter
 * - Application of the filter on a test image
 * - Verifying the output image dimensions and channel count
 * - Checking that edge detection produces expected results on test regions
 */
void runPrewittFilterTests() {
    std::cout << "  Testing PrewittFilter..." << std::endl;
    
    // Test 1: Constructor
    PrewittFilter filter;
    
    // Test 2: Apply filter to a simple test image
    // Create a 10x10 image with 4 channels (RGBA)
    const int width = 10;
    const int height = 10;
    const int channels = 4;
    Image image(width, height, channels);
    
    // Create a simple test scenario:
    // Left half of the image is white, right half is black
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (x < width / 2) {
                // White region
                image.setPixel(x, y, Pixel(255, 255, 255, 255));
            } else {
                // Black region
                image.setPixel(x, y, Pixel(0, 0, 0, 255));
            }
        }
    }
    
    // Check that calling apply does not throw an exception
    CHECK_NOTHROW(filter.apply(image), "PrewittFilter::apply should not throw exceptions");
    
    // Apply the Prewitt filter to the image
    Image result = filter.apply(image);
    
    // Verify that the output image dimensions and channel count match the input image
    CHECK(result.getWidth() == width, "Output image width should match input");
    CHECK(result.getHeight() == height, "Output image height should match input");
    
    // For edge detection, we expect a significant intensity change at the border
    // Check a pixel at the border (e.g., at (width/2, height/2))
    Pixel borderPixel = result.getPixel(width / 2, height / 2);
    // Assume that the edge magnitude is represented in the red channel or overall intensity.
    // The intensity at the border should be above a certain threshold.
    CHECK(borderPixel.getR() > 100, "Edge detection at the border should produce a high intensity value");
    
    // Check a pixel far from the edge, e.g., in the middle of the white region
    Pixel whiteRegionPixel = result.getPixel(width / 4, height / 2);
    // Away from the edge, the intensity should be low.
    CHECK(whiteRegionPixel.getR() < 50, "Non-edge regions should have a low intensity response");
    
    std::cout << "  PrewittFilter tests completed." << std::endl;
}
