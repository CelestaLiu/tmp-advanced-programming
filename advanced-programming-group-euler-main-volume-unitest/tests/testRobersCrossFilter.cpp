/**
 * @file testRobertsCrossFilter.cpp
 * @brief Tests for the RobertsCrossFilter class
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

#include "TestCounters.h"
#include "../src/filter2D/RobertsCrossFilter.h"
#include "../src/Volume.h"
#include "../src/Pixel.h"
#include <iostream>
#include <memory>
#include <vector>

/**
 * @brief Run tests for the RobertsCrossFilter class
 * 
 * This function tests the basic functionality of the RobertsCrossFilter,
 * including construction and application to a simple image for edge detection.
 */
void runRobertsCrossFilterTests() {
    std::cout << "  Testing RobertsCrossFilter..." << std::endl;
    
    // Test 1: Constructor
    RobertsCrossFilter filter;
    CHECK(filter.getName() == "Roberts Cross", "RobertsCrossFilter constructor should set correct name");
    
    // Test 2: Apply to a simple image
    // Create a 10x10 test image with a white background and a black square in the middle
    const int width = 10;
    const int height = 10;
    const int channels = 4; // RGBA
    Image image(width, height, channels);
    
    // Initialize the image with a white background
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            image.setPixel(x, y, Pixel(255, 255, 255, 255));
        }
    }
    
    // Draw a 3x3 black square in the middle of the image
    for (int y = 3; y < 6; y++) {
        for (int x = 3; x < 6; x++) {
            image.setPixel(x, y, Pixel(0, 0, 0, 255));
        }
    }
    
    // Test that applying the filter does not throw exceptions
    CHECK_NOTHROW(filter.apply(image), "RobertsCrossFilter::apply should not throw exceptions");
    
    // Apply the Roberts Cross filter to detect edges
    Image result = filter.apply(image);
    
    // Verify that the output image has the same dimensions and channels as the input image
    CHECK(result.getWidth() == width, "Output image width should match input");
    CHECK(result.getHeight() == height, "Output image height should match input");
    CHECK(result.getChannels() == channels, "Output image channels should match input");
    
    // Verify alpha channel preservation for a sample pixel (e.g., at (5,5))
    Pixel originalPixel = image.getPixel(5, 5);
    Pixel resultPixel = result.getPixel(5, 5);
    CHECK(resultPixel.getA() == originalPixel.getA(), "Alpha channel should be preserved");
    
    // Since the filter converts the image to greyscale and highlights edges,
    // uniform regions should yield low edge responses.
    // Check a pixel in a uniform white region (e.g., at (1,1))
    resultPixel = result.getPixel(1, 1);
    CHECK(resultPixel.getR() < 10, "Uniform white area should have low edge response");
    
    // Check a pixel in a uniform black region (inside the square, e.g., at (4,4))
    resultPixel = result.getPixel(4, 4);
    CHECK(resultPixel.getR() < 10, "Uniform black area should have low edge response");
    
    // The border of the black square should produce a strong edge response.
    // Check a pixel at the edge of the square (e.g., at (3,3))
    resultPixel = result.getPixel(3, 3);
    CHECK(resultPixel.getR() > 50, "Edge area should produce a strong edge response");
    
    std::cout << "  RobertsCrossFilter tests completed." << std::endl;
}
