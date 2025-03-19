/**
 * @file testSharpeningFilter.cpp
 * @brief Tests for the SharpeningFilter class
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
 #include "SharpeningFilter.h"
 #include "Image.h"
 #include "Pixel.h"
 #include <iostream>
 
 /**
  * @brief Run tests for the SharpeningFilter class
  * 
  * This function tests the basic functionality of the SharpeningFilter,
  * including construction and application to a simple image.
  */
 void runSharpeningFilterTests() {
     std::cout << "  Testing SharpeningFilter..." << std::endl;
     
     // Test 1: Constructor
     SharpeningFilter filter;
     CHECK(filter.getName() == "Sharpening", "SharpeningFilter constructor should set correct name");
     
     // Test 2: Apply to a simple image
     // Create a 10x10 test image with a black square in the middle
     const int width = 10;
     const int height = 10;
     const int channels = 4; // RGBA
     Image image(width, height, channels);
     
     // Initialize with white background
     for (int y = 0; y < height; y++) {
         for (int x = 0; x < width; x++) {
             image.setPixel(x, y, Pixel(255, 255, 255, 255));
         }
     }
     
     // Draw a 3x3 black square in the middle
     for (int y = 3; y < 6; y++) {
         for (int x = 3; x < 6; x++) {
             image.setPixel(x, y, Pixel(0, 0, 0, 255));
         }
     }
     
     // Test applying the filter without exceptions
     CHECK_NOTHROW(filter.apply(image), "SharpeningFilter::apply should not throw exceptions");
     
     // Apply sharpening filter
     Image result = filter.apply(image);
     
     // Verify dimensions
     CHECK(result.getWidth() == width, "Output image width should match input");
     CHECK(result.getHeight() == height, "Output image height should match input");
     CHECK(result.getChannels() == channels, "Output image channels should match input");
     
     // Verify alpha channel preservation for a sample pixel
     Pixel originalPixel = image.getPixel(5, 5);
     Pixel resultPixel = result.getPixel(5, 5);
     CHECK(resultPixel.getA() == originalPixel.getA(), "Alpha channel should be preserved");
     
     // Verify the black square is still present (color values should remain relatively low)
     resultPixel = result.getPixel(4, 4); // Center of the black square
     CHECK(resultPixel.getR() < 128, "Sharpened black area should remain relatively dark");
     
     // Verify the white area is still present (color values should remain relatively high)
     resultPixel = result.getPixel(1, 1); // Corner (white area)
     CHECK(resultPixel.getR() > 200, "Sharpened white area should remain relatively bright");
     
     std::cout << "  SharpeningFilter tests completed." << std::endl;
 }