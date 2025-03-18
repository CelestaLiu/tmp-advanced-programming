/**
 * @file Image.cpp
 * @brief Implementation of the abstract Image class
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




// Include STB image libraries
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "Image.h"
#include <stdexcept>


// *******************************************************************************************
// -------------------------------------------------------------------------------------------
// CONSTRUCTORS, DESTRUCTOR & CLONE
// -------------------------------------------------------------------------------------------
// *******************************************************************************************

// Constructor with dimensions
Image::Image(int width, int height, int channels)
    : DataContainer(width, height), channels(channels) {
    // Initialize pixels with black color (from pixel)
    pixels.resize(height, std::vector<Pixel>(width, Pixel()));
}

// Constructor from file
Image::Image(const std::string& filename)
    : DataContainer(1, 1) {  // Initialize with 0 dimensions, will be updated in loadFromFile
    if (!loadFromFile(filename)) {
        throw std::runtime_error("Failed to load image from file: " + filename);
    }
}
// Create deep copy
std::unique_ptr<DataContainer> Image::clone() const {
    //std::unique_ptr<DataContainer> ensures automatic memory management--> no need to manually delete object later.
    auto copy = std::make_unique<Image>(width, height, channels);
    // note that the channels are stored in the Pixel object, so no need for an extra loop to explicitly copy them over.
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            copy->setPixel(x, y, getPixel(x, y));
        }
    }
    return copy;
}

// *******************************************************************************************
// -------------------------------------------------------------------------------------------
// GETTERS & SETTERS
// -------------------------------------------------------------------------------------------
// *******************************************************************************************

// Get pixel at position
Pixel Image::getPixel(int x, int y) const {
    // Check bounds  - returns a boolean true/false value 
    if (!isInBounds(x, y)) {
        throw std::out_of_range("Pixel coordinates out of bounds");
    }
    // Returning y-th row and x-th column pixel because Pixels is a 2D vector stored in row-major order.
    return pixels[y][x];
}

// Set pixel at position
void Image::setPixel(int x, int y, const Pixel& pixel) {
    // Check bounds  - returns a boolean true/false value 
    if (!isInBounds(x, y)) {
        throw std::out_of_range("Pixel coordinates out of bounds");
    }
    // Returning y-th row and x-th column pixel because Pixels is a 2D vector stored in row-major order.
    pixels[y][x] = pixel;
}

// Get number of channels
int Image::getChannels() const {
    return channels;
}

// *******************************************************************************************
// -------------------------------------------------------------------------------------------
// LOADING & SAVING
// -------------------------------------------------------------------------------------------
// *******************************************************************************************

// Save to file
bool Image::saveToFile(const std::string& filename) const {
    // Create data array for stb_image_write (1D array)
    unsigned char* data = new unsigned char[width * height * channels];
    
    // Copy pixel data to array
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Pixel p = pixels[y][x];
            int index = (y * width + x) * channels;
            data[index] = p.getR();
            data[index + 1] = p.getG();
            data[index + 2] = p.getB();
            if (channels == 4) {
                data[index + 3] = p.getA();
            }
        }
    }
    // Write to file
    int success = stbi_write_png(filename.c_str(), width, height, channels, data, 0);
    // Clean up
    delete[] data;
    return success != 0;
}

// Load from file
bool Image::loadFromFile(const std::string& filename) {
    int w, h, c;
    // Using stb_image to load image data
    unsigned char* data = stbi_load(filename.c_str(), &w, &h, &c, 0);
    
    if (!data) {
        return false;
    }
    
    // Update dimensions and channels
    width = w;
    height = h;
    channels = c;
    
    // Resize pixels array
    pixels.resize(height, std::vector<Pixel>(width));
    
    // Copy data to pixels
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int index = (y * width + x) * channels;
            unsigned char r = data[index];
            unsigned char g = (channels > 1) ? data[index + 1] : r;
            unsigned char b = (channels > 2) ? data[index + 2] : r;
            unsigned char a = (channels > 3) ? data[index + 3] : 255;
            pixels[y][x] = Pixel(r, g, b, a);
        }
    }
    
    // Free stb_image data
    stbi_image_free(data);
    
    return true;
}

// *******************************************************************************************
// -------------------------------------------------------------------------------------------
// IMAGE MANIPULATION
// -------------------------------------------------------------------------------------------
// *******************************************************************************************

// Apply filter (placeholder implementation)
std::unique_ptr<DataContainer> Image::applyFilter(const std::string& filterName, 
    const std::string& parameters) const {
// Simple placeholder - just return a clone for now
return clone();
}

// Convert to grayscale
Image Image::toGrayscale() const {
    // Create new image with same dimensions, but one channel
    Image grayscale(width, height, 1);
    // Convert each pixel to grayscale
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            grayscale.setPixel(x, y, getPixel(x, y).toGrayscale());
        }
    }
    return grayscale;
}
