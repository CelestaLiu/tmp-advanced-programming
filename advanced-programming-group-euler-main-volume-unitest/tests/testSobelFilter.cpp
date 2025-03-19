#include "../src/filter2D/SobelFilter.h"
#include "../src/Image.h"
#include "../src/Pixel.h"
#include "TestCounters.h"
#include <iostream>
#include <cmath>

/**
 * @brief Test that a flat image produces no edge response.
 *
 * A uniform (flat) image should result in near zero edge intensities
 * after applying the SobelFilter.
 */
void test_sobel_flat_image() {
    // Create a 3x3 flat image with all pixels set to (128, 128, 128)
    Image flatImage(3, 3, 4);
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            flatImage.setPixel(x, y, Pixel(128, 128, 128));
        }
    }
    
    SobelFilter filter;
    Image result = filter.apply(flatImage);
    
    // Verify that every pixel in the result has an intensity of 0 (no edge)
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            CHECK(result.getPixel(x, y).getR() == 0, "Flat image produces zero edge response");
        }
    }
}

/**
 * @brief Test that the SobelFilter correctly detects an edge.
 *
 * This test creates a 3x3 image with a horizontal edge:
 * - Top row: intensity 0
 * - Bottom two rows: intensity 255
 * The filter should detect a strong edge and normalize the maximum edge response to 255.
 */
void test_sobel_edge_detection() {
    // Create a 3x3 image with a horizontal edge:
    // Row 0: all 0; Rows 1 and 2: all 255
    Image edgeImage(3, 3, 4);
    
    // Set top row to 0
    for (int x = 0; x < 3; x++) {
        edgeImage.setPixel(x, 0, Pixel(0, 0, 0));
    }
    // Set rows 1 and 2 to 255
    for (int y = 1; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            edgeImage.setPixel(x, y, Pixel(255, 255, 255));
        }
    }
    
    SobelFilter filter;
    Image result = filter.apply(edgeImage);
    
    // Find the maximum edge response in the resulting image
    int maxVal = 0;
    for (int y = 0; y < result.getHeight(); y++) {
        for (int x = 0; x < result.getWidth(); x++) {
            int val = result.getPixel(x, y).getR();
            if (val > maxVal) {
                maxVal = val;
            }
        }
    }
    CHECK(maxVal == 255, "Maximum edge response normalized to 255");
}

/**
 * @brief Test that the output image dimensions match the input image dimensions.
 *
 * This ensures that the SobelFilter maintains the size of the input image.
 */
void test_sobel_image_dimensions() {
    // Create a 4x5 test image with arbitrary pixel intensities
    Image testImage(4, 5, 4);
    for (int y = 0; y < 5; y++) {
        for (int x = 0; x < 4; x++) {
            int intensity = x * 50;
            testImage.setPixel(x, y, Pixel(intensity, intensity, intensity));
        }
    }
    
    SobelFilter filter;
    Image result = filter.apply(testImage);
    
    CHECK(result.getWidth() == testImage.getWidth(), "Output width matches input width");
    CHECK(result.getHeight() == testImage.getHeight(), "Output height matches input height");
}

/**
 * @brief Run all tests for the SobelFilter.
 */
void runSobelFilterTests() {
    std::cout << "\n=== Running SobelFilter Tests ===\n";
    test_sobel_flat_image();
    test_sobel_edge_detection();
    test_sobel_image_dimensions();
    // std::cout << "\nSobelFilter Tests Summary: " << passed << " passed, " << failed << " failed\n";
}

