/**
 * @file Pixel.cpp
 * @brief Implementation of the Pixel class
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

#include "Pixel.h"
#include <algorithm> // For clamp
#include <cmath>     // For mathematical functions

// *******************************************************************************************
// -------------------------------------------------------------------------------------------
// CONSTRUCTORS, DESTRUCTOR & CLONE
// -------------------------------------------------------------------------------------------
// *******************************************************************************************

// Default constructor
Pixel::Pixel() : r(0), g(0), b(0), a(255) {}

// Constructor with RGB(A) values
Pixel::Pixel(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
    : r(r), g(g), b(b), a(a) {}

// Copy constructor
Pixel::Pixel(const Pixel& other) 
    : r(other.r), g(other.g), b(other.b), a(other.a) {}

// Destructor (not much to do here since we use primitive types)
Pixel::~Pixel() {}

// *******************************************************************************************
// -------------------------------------------------------------------------------------------
// GETTERS & SETTERS
// -------------------------------------------------------------------------------------------
// *******************************************************************************************

// Getters
unsigned char Pixel::getR() const { return r; }
unsigned char Pixel::getG() const { return g; }
unsigned char Pixel::getB() const { return b; }
unsigned char Pixel::getA() const { return a; }

// Setters
void Pixel::setR(unsigned char r) { this->r = r; }
void Pixel::setG(unsigned char g) { this->g = g; }
void Pixel::setB(unsigned char b) { this->b = b; }
void Pixel::setA(unsigned char a) { this->a = a; }

// Set all RGBA values at once (default alpha to 255)
void Pixel::setRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}
// Calculate luminance using the specified formula from project requirements
float Pixel::getLuminance() const {
    return 0.2126f * r + 0.7152f * g + 0.0722f * b;
}

// *******************************************************************************************
// -------------------------------------------------------------------------------------------
// IMAGE MANIPULATION
// -------------------------------------------------------------------------------------------
// *******************************************************************************************

// Convert to greyscale
Pixel Pixel::toGreyscale() const {
    // Calculate luminance and round to nearest integer
    unsigned char grey = static_cast<unsigned char>(std::round(getLuminance()));
    return Pixel(grey, grey, grey, a);
}
// Convert RGB to HSV
void Pixel::RGBtoHSV(float& h, float& s, float& v) const {
    // Normalize RGB values to range [0,1]
    float r_norm = r / 255.0f;
    float g_norm = g / 255.0f;
    float b_norm = b / 255.0f;
    
    // Find maximum and minimum RGB components
    float cmax = std::max({r_norm, g_norm, b_norm});
    float cmin = std::min({r_norm, g_norm, b_norm});
    float delta = cmax - cmin;
    
    // Calculate hue
    if (delta == 0) {
        h = 0; // No color, hue is undefined, default to 0
    } else if (cmax == r_norm) {
        // Red is max component, hue between yellow and magenta
        h = 60.0f * fmodf(((g_norm - b_norm) / delta), 6.0f);
    } else if (cmax == g_norm) {
        // Green is max component, hue between cyan and yellow
        h = 60.0f * (((b_norm - r_norm) / delta) + 2.0f);
    } else { // cmax == b_norm
        // Blue is max component, hue between magenta and cyan
        h = 60.0f * (((r_norm - g_norm) / delta) + 4.0f);
    }
    
    // Make sure hue is non-negative
    if (h < 0) {
        h += 360.0f;
    }
    
    // Calculate saturation
    s = (cmax == 0) ? 0 : (delta / cmax);
    
    // Value is the maximum component
    v = cmax;
}
// Set pixel values from HSV
void Pixel::HSVtoRGB(float h, float s, float v) {
    // Handle grayscale case
    if (s <= 0.0f) {
        // Achromatic (gray)
        r = g = b = static_cast<unsigned char>(std::round(v * 255.0f));
        return;
    }
    
    // Normalize hue to [0,360)
    h = fmodf(h, 360.0f);
    if (h < 0) h += 360.0f;
    h /= 60.0f; // sector 0 to 5
    
    int i = static_cast<int>(h);
    float f = h - i; // factorial part of h
    
    // Calculate color components based on the sector
    float p = v * (1 - s);
    float q = v * (1 - s * f);
    float t = v * (1 - s * (1 - f));
    
    float r_norm, g_norm, b_norm;
    
    switch (i) {
        case 0:
            r_norm = v; g_norm = t; b_norm = p;
            break;
        case 1:
            r_norm = q; g_norm = v; b_norm = p;
            break;
        case 2:
            r_norm = p; g_norm = v; b_norm = t;
            break;
        case 3:
            r_norm = p; g_norm = q; b_norm = v;
            break;
        case 4:
            r_norm = t; g_norm = p; b_norm = v;
            break;
        default: // case 5
            r_norm = v; g_norm = p; b_norm = q;
            break;
    }
    
    // Convert back to 8-bit RGB
    r = static_cast<unsigned char>(std::round(r_norm * 255.0f));
    g = static_cast<unsigned char>(std::round(g_norm * 255.0f));
    b = static_cast<unsigned char>(std::round(b_norm * 255.0f));
}
// Convert RGB to HSL
void Pixel::RGBtoHSL(float& h, float& s, float& l) const {
    // Normalize RGB values to range [0,1]
    float r_norm = r / 255.0f;
    float g_norm = g / 255.0f;
    float b_norm = b / 255.0f;
    
    // Find maximum and minimum RGB components
    float cmax = std::max({r_norm, g_norm, b_norm});
    float cmin = std::min({r_norm, g_norm, b_norm});
    float delta = cmax - cmin;
    
    // Calculate lightness
    l = (cmax + cmin) / 2.0f;
    
    // Calculate saturation
    if (delta == 0) {
        s = 0; // No color, saturation is 0
    } else {
        // Saturation formula depends on lightness
        s = (l <= 0.5f) ? (delta / (cmax + cmin)) : (delta / (2.0f - cmax - cmin));
    }
    
    // Calculate hue
    if (delta == 0) {
        h = 0; // No color, hue is undefined, default to 0
    } else if (cmax == r_norm) {
        // Red is max component
        h = 60.0f * fmodf(((g_norm - b_norm) / delta), 6.0f);
    } else if (cmax == g_norm) {
        // Green is max component
        h = 60.0f * (((b_norm - r_norm) / delta) + 2.0f);
    } else { // cmax == b_norm
        // Blue is max component
        h = 60.0f * (((r_norm - g_norm) / delta) + 4.0f);
    }
    
    // Make sure hue is non-negative
    if (h < 0) {
        h += 360.0f;
    }
}
// Set pixel values from HSL
void Pixel::HSLtoRGB(float h, float s, float l) {
    // Handle grayscale case
    if (s <= 0.0f) {
        // Achromatic (gray)
        r = g = b = static_cast<unsigned char>(std::round(l * 255.0f));
        return;
    }
    
    // Normalize hue to [0,360)
    h = fmodf(h, 360.0f);
    if (h < 0) h += 360.0f;
    
    // Helper values
    float q = (l < 0.5f) ? (l * (1.0f + s)) : (l + s - l * s);
    float p = 2.0f * l - q;
    
    // Helper function to convert hue to RGB component
    auto hue_to_rgb = [&](float h) {
        h = fmodf(h, 1.0f);
        if (h < 0) h += 1.0f;
        
        if (h < 1.0f / 6.0f) return p + (q - p) * 6.0f * h;
        if (h < 1.0f / 2.0f) return q;
        if (h < 2.0f / 3.0f) return p + (q - p) * (2.0f / 3.0f - h) * 6.0f;
        return p;
    };
    
    // Calculate RGB components
    float r_norm = hue_to_rgb((h / 360.0f) + 1.0f / 3.0f);
    float g_norm = hue_to_rgb(h / 360.0f);
    float b_norm = hue_to_rgb((h / 360.0f) - 1.0f / 3.0f);
    
    // Convert back to 8-bit RGB
    r = static_cast<unsigned char>(std::round(r_norm * 255.0f));
    g = static_cast<unsigned char>(std::round(g_norm * 255.0f));
    b = static_cast<unsigned char>(std::round(b_norm * 255.0f));
}
// Adjust brightness
Pixel Pixel::adjustBrightness(int value) const {
    // Clamp function to ensure values stay in 0-255 range
    auto clamp = [](int val) {
        return static_cast<unsigned char>(std::clamp(val, 0, 255));
    };
    // Create a new pixel with adjusted RGB values but same alpha
    return Pixel(
        clamp(r + value),
        clamp(g + value),
        clamp(b + value),
        a
    );
}

// *******************************************************************************************
// -------------------------------------------------------------------------------------------
// OPERATOR OVERRIDES 
// -------------------------------------------------------------------------------------------
// *******************************************************************************************

// Equality operator
bool Pixel::operator==(const Pixel& other) const {
    return (r == other.r && g == other.g && b == other.b && a == other.a);
}
// Inequality operator
bool Pixel::operator!=(const Pixel& other) const {
    return !(*this == other);
}
// Assignment operator
Pixel& Pixel::operator=(const Pixel& other) {
    if (this != &other) {
        r = other.r;
        g = other.g;
        b = other.b;
        a = other.a;
    }
    return *this;
}
