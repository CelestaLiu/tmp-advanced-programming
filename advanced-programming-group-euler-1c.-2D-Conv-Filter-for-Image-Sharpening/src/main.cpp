// A minimal example file showing how to include the stb image headers,
// and how to read and write image files using these headers.
//
// Compilation with (e.g.): g++-14 main.cpp -o main
//
// Written by T.M. Davison (2023-25)

#include "stb_image.h"
#include "stb_image_write.h"
#include "SimpleFilters.h"
#include "Pixel.h"
#include "Image.h"
#include <iostream>
#include <cassert>
#include <string>
#include <filesystem>


// #include "Filter.h"
// #include "Volume.h"
// #include "Projection.h"
// #include "Slice.h"

// Test functionality of the Pixel class
void testPixelClass() {
    std::cout << "=== Testing Pixel Class ===" << std::endl;
    
    // Test constructors and getters
    Pixel p1;
    std::cout << "Default pixel: R=" << (int)p1.getR() << ", G=" << (int)p1.getG() 
              << ", B=" << (int)p1.getB() << ", A=" << (int)p1.getA() << std::endl;
    
    Pixel p2(255, 0, 0);
    std::cout << "Red pixel: R=" << (int)p2.getR() << ", G=" << (int)p2.getG() 
              << ", B=" << (int)p2.getB() << ", A=" << (int)p2.getA() << std::endl;
    
    // Test setters
    p1.setRGBA(0, 255, 0, 128);
    std::cout << "Modified pixel: R=" << (int)p1.getR() << ", G=" << (int)p1.getG() 
              << ", B=" << (int)p1.getB() << ", A=" << (int)p1.getA() << std::endl;
    
    // Test grayscale conversion
    Pixel gray = p2.toGrayscale();
    std::cout << "Grayscale conversion: R=" << (int)gray.getR() << ", G=" << (int)gray.getG() 
              << ", B=" << (int)gray.getB() << std::endl;
    
    // Test brightness adjustment
    Pixel brighter = p2.adjustBrightness(50);
    std::cout << "After brightening: R=" << (int)brighter.getR() << ", G=" << (int)brighter.getG() 
              << ", B=" << (int)brighter.getB() << std::endl;
    
    // Test HSV conversion
    float h, s, v;
    p2.RGBtoHSV(h, s, v);
    std::cout << "HSV values: H=" << h << ", S=" << s << ", V=" << v << std::endl;
    
    Pixel p3;
    p3.HSVtoRGB(h, s, v);
    std::cout << "HSV back to RGB: R=" << (int)p3.getR() << ", G=" << (int)p3.getG() 
              << ", B=" << (int)p3.getB() << std::endl;
    
    // Test equality
    std::cout << "Pixel equality test: " << (p2 == p3 ? "Equal" : "Not equal") << std::endl;
    
    std::cout << std::endl;
}

// Test functionality of the Image class
void testImageClass() {
    std::cout << "=== Testing Image Class ===" << std::endl;
    
    // Test image creation
    Image img(3, 3);
    std::cout << "Created " << img.getWidth() << "x" << img.getHeight() 
              << " image with " << img.getChannels() << " channels" << std::endl;
    
    // Test setting and getting pixels
    img.setPixel(0, 0, Pixel(255, 0, 0));
    img.setPixel(1, 0, Pixel(0, 255, 0));
    img.setPixel(2, 0, Pixel(0, 0, 255));
    
    img.setPixel(0, 1, Pixel(255, 255, 0));
    img.setPixel(1, 1, Pixel(255, 0, 255));
    img.setPixel(2, 1, Pixel(0, 255, 255));
    
    img.setPixel(0, 2, Pixel(255, 255, 255));
    img.setPixel(1, 2, Pixel(128, 128, 128));
    img.setPixel(2, 2, Pixel(0, 0, 0));
    
    // Save test image
    std::cout << "Saving test image to test_image.png" << std::endl;
    if (img.saveToFile("test_image.png")) {
        std::cout << "Save successful" << std::endl;
    } else {
        std::cout << "Save failed" << std::endl;
    }
    
    // Test cloning
    std::unique_ptr<DataContainer> clone = img.clone();
    std::cout << "Cloned image size: " << clone->getWidth() << "x" << clone->getHeight() << std::endl;
    
    // Test grayscale conversion
    Image grayscale = img.toGrayscale();
    std::cout << "Converted to grayscale image with " << grayscale.getChannels() << " channels" << std::endl;
    grayscale.saveToFile("test_grayscale.png");
    
    std::cout << std::endl;
}

// Test loading real images using stb_image
void testLoadRealImage(const std::string& filename) {
    std::cout << "=== Testing Real Image Loading ===" << std::endl;
    
    try {
        // Load image
        Image img(filename);
        std::cout << "Successfully loaded image: " << filename << std::endl;
        std::cout << "Image dimensions: " << img.getWidth() << "x" << img.getHeight() 
                  << ", channels: " << img.getChannels() << std::endl;
        
        // Create grayscale version
        Image grayscale = img.toGrayscale();
        std::string outputFilename = "gray_" + filename.substr(filename.find_last_of("/\\") + 1);
        grayscale.saveToFile(outputFilename);
        std::cout << "Saved grayscale version to: " << outputFilename << std::endl;
        
        // Create brightness enhanced version
        std::unique_ptr<Image> brightCopy = std::make_unique<Image>(img.getWidth(), img.getHeight(), img.getChannels());
        for (int y = 0; y < img.getHeight(); ++y) {
            for (int x = 0; x < img.getWidth(); ++x) {
                brightCopy->setPixel(x, y, img.getPixel(x, y).adjustBrightness(30));
            }
        }
        
        std::string brightFilename = "bright_" + filename.substr(filename.find_last_of("/\\") + 1);
        brightCopy->saveToFile(brightFilename);
        std::cout << "Saved brightness enhanced version to: " << brightFilename << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
    
    std::cout << std::endl;
}


int main(int argc, char* argv[]) {
    // Test Pixel class
    testPixelClass();
    
    // Test Image class
    testImageClass();
    
    // Test loading real images (if provided via command line)
    if (argc > 1) {
        testLoadRealImage(argv[1]);
    } else {
        std::cout << "Tip: You can provide an image path as a command line argument to test loading real images" << std::endl;
    }
    
    try {
        // Load test image
        std::string inputFilename = "Images/bourton.png";
        Image img(inputFilename);

        // TEST 1: APPLY GRAYSCALE FILTER
        SimpleFilters grayscaleFilter("Grayscale");
        Image grayscaleImg = grayscaleFilter.apply(img);
        grayscaleImg.saveToFile("build/output/output_grayscale.png");
        std::cout << "Grayscale filter applied successfully!\n";

        // TEST 2: APPLY BRIGHTNESS ADJUSTMENT
        SimpleFilters brightnessFilter("Brightness");
        brightnessFilter.setParameter("brightness", 150);
        Image brightImg = brightnessFilter.apply(img);
        brightImg.saveToFile("build/output/output_brightness.png");
        std::cout << "Brightness filter applied successfully!\n";

        // TEST 3: APPLY HISTOGRAM EQUALIZATION
        SimpleFilters histEqFilter("HistogramEqualization");
        Image histEqImg = histEqFilter.apply(img);
        histEqImg.saveToFile("build/output/output_histogram_equalization.png");
        std::cout << "Histogram Equalization filter applied successfully!\n";

        // TEST 4: APPLY THRESHOLDING
        SimpleFilters thresholdFilter("Thresholding");
        thresholdFilter.setParameter("threshold", 128);
        Image thresholdImg = thresholdFilter.apply(img);
        thresholdImg.saveToFile("build/output/output_thresholding.png");
        std::cout << "Thresholding filter applied successfully!\n";

        // TEST 5: APPLY SALT AND PEPPER NOISE
        SimpleFilters noiseFilter("SaltAndPepperNoise");
        noiseFilter.setParameter("noise", 5.0f); // 5% noise
        Image noisyImg = noiseFilter.apply(img);
        noisyImg.saveToFile("build/output/output_salt_pepper.png");
        std::cout << "Salt and Pepper Noise filter applied successfully!\n";

        std::cout << "All tests completed successfully!\n";

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}