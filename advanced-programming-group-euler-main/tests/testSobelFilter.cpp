/**
 * @file testSobelFilter.cpp
 * @brief Tests for the SobelFilter class functionality
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
#include "../src/filter2D/SobelFilter.h"
#include "../src/Image.h"
#include "../src/Pixel.h"
#include <iostream>

/**
 * @brief Runs tests for the SobelFilter class
 *
 * This function tests the basic functionality of the SobelFilter, including:
 * - Constructing the filter.
 * - Applying the filter to a test image.
 * - Verifying that the output image dimensions and channel count match the input.
 * - Checking that edge detection produces a high intensity response at the edge.
 */
void runSobelFilterTests() {
    std::cout << "  Testing SobelFilter..." << std::endl;
    
    // Test 1: Constructor
    SobelFilter filter;
    
    // Test 2: Create a test image with a vertical edge
    // Create a 10x10 image with 4 channels (RGBA)
    const int width = 10;
    const int height = 10;
    const int channels = 4;
    Image image(width, height, channels);
    
    // Fill the image: left half is white and right half is black
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (x < width / 2) {
                image.setPixel(x, y, Pixel(255, 255, 255, 255));
            } else {
                image.setPixel(x, y, Pixel(0, 0, 0, 255));
            }
        }
    }
    
    // Ensure that applying the filter does not throw an exception
    CHECK_NOTHROW(filter.apply(image), "SobelFilter::apply should not throw exceptions");
    
    // Apply the Sobel filter to the image
    Image result = filter.apply(image);
    
    // Verify that the output image dimensions and channel count match the input image
    CHECK(result.getWidth() == width, "Output image width should match input");
    CHECK(result.getHeight() == height, "Output image height should match input");

    // Check edge detection response:
    // The edge between the white and black regions should produce a high intensity response.
    // Sample a pixel at the vertical boundary, e.g., at (width/2, height/2)
    Pixel edgePixel = result.getPixel(width / 2, height / 2);
    CHECK(edgePixel.getR() > 100, "Edge detection at the boundary should produce a high intensity value");
    
    // Verify that a pixel away from the edge (in the white region) has a low intensity response.
    Pixel nonEdgePixel = result.getPixel(width / 4, height / 2);
    CHECK(nonEdgePixel.getR() < 50, "Non-edge regions should have a low intensity response");
    
    std::cout << "  SobelFilter tests completed." << std::endl;
}
