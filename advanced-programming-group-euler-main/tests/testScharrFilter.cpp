/**
 * @file testScharrFilter.cpp
 * @brief Tests for the ScharrFilter class functionality
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

#include "TestCounters.h"      // Include test counters and assertion macros
#include "../src/filter2D/ScharrFilter.h"
#include "../src/Image.h"
#include "../src/Pixel.h"
#include <iostream>

/**
 * @brief Runs tests for the ScharrFilter class
 *
 * This function tests the basic functionality of the ScharrFilter, including:
 * - Constructing the filter.
 * - Applying the filter to a test image.
 * - Verifying that the output image dimensions and channel count match the input.
 * - Checking that edge detection produces a significant response at the edge.
 */
void runScharrFilterTests() {
    std::cout << "  Testing ScharrFilter..." << std::endl;
    
    // Test 1: Constructor
    ScharrFilter filter;
    
    // Test 2: Apply filter to a test image
    // Create a 10x10 image with 4 channels (RGBA)
    const int width = 10;
    const int height = 10;
    const int channels = 4;
    Image image(width, height, channels);
    
    // Create a test image with a horizontal edge:
    // Top half is white and bottom half is black.
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (y < height / 2) {
                image.setPixel(x, y, Pixel(255, 255, 255, 255));
            } else {
                image.setPixel(x, y, Pixel(0, 0, 0, 255));
            }
        }
    }
    
    // Check that applying the filter does not throw an exception
    CHECK_NOTHROW(filter.apply(image), "ScharrFilter::apply should not throw exceptions");
    
    // Apply the Scharr filter to the image
    Image result = filter.apply(image);
    
    // Verify that the output image dimensions and channel count match the input image
    CHECK(result.getWidth() == width, "Output image width should match input");
    CHECK(result.getHeight() == height, "Output image height should match input");

    // Check edge detection response:
    // Expect a strong edge response around the horizontal boundary.
    Pixel edgePixel = result.getPixel(width / 2, height / 2);
    CHECK(edgePixel.getR() > 100, "Edge detection at the horizontal edge should produce a high intensity value");
    
    // Verify that a pixel away from the edge (e.g., in the top-left white region) has a low intensity response
    Pixel nonEdgePixel = result.getPixel(1, 1);
    CHECK(nonEdgePixel.getR() < 50, "Non-edge regions should have a low intensity response");
    
    std::cout << "  ScharrFilter tests completed." << std::endl;
}
