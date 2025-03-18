/**
 * @file Filter.cpp
 * @brief Implementation of the SimpleFilters class
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


#include "SimpleFilters.h"
#include <algorithm>  // For min/max operations
#include <random>     // For noise generation

// *******************************************************************************************
// -------------------------------------------------------------------------------------------
// CONSTRUCTORS & DESTRUCTOR
// -------------------------------------------------------------------------------------------
// *******************************************************************************************

SimpleFilters::SimpleFilters(const std::string& filterName) 
    : Filter(filterName) {}

// *******************************************************************************************
// -------------------------------------------------------------------------------------------
// APPLY FUNCTION (DETERMINES WHICH FILTER TO CALL)
// -------------------------------------------------------------------------------------------
// *******************************************************************************************

Image SimpleFilters::apply(const Image& input) {
    if (name == "Grayscale") {
        return applyGrayscale(input);
    } else if (name == "Brightness") {
        int brightness = std::any_cast<int>(parameters["brightness"]);
        return applyBrightness(input, brightness);
    } else if (name == "HistogramEqualization") {
        return applyHistogramEqualization(input);
    } else if (name == "Thresholding") {
        int threshold = std::any_cast<int>(parameters["threshold"]);
        return applyThresholding(input, threshold);
    } else if (name == "SaltAndPepperNoise") {
        float noisePercentage = std::any_cast<float>(parameters["noise"]);
        return applySaltAndPepperNoise(input, noisePercentage);
    }
    throw std::invalid_argument("Unknown filter: " + name);
}

// *******************************************************************************************
// -------------------------------------------------------------------------------------------
// INDIVIDUAL FILTER IMPLEMENTATIONS
// -------------------------------------------------------------------------------------------
// *******************************************************************************************

Image SimpleFilters::applyGrayscale(const Image& input) {
    Image grayscale(input.getWidth(), input.getHeight(), 1);
    for (int y = 0; y < input.getHeight(); ++y) {
        for (int x = 0; x < input.getWidth(); ++x) {
            grayscale.setPixel(x, y, input.getPixel(x, y).toGrayscale());
        }
    }
    return grayscale;
}

Image SimpleFilters::applyBrightness(const Image& input, int brightness) {
    Image output = input;
    for (int y = 0; y < input.getHeight(); ++y) {
        for (int x = 0; x < input.getWidth(); ++x) {
            Pixel p = input.getPixel(x, y).adjustBrightness(brightness);
            output.setPixel(x, y, p);
        }
    }
    return output;
}

/**
 * @brief Apply Histogram Equalization to a grayscale image.
 * @param input Grayscale image (1 channel).
 * @return Image with equalized histogram.
 */
Image SimpleFilters::applyGrayscaleHistogramEqualization(const Image& input) {
    int width = input.getWidth();
    int height = input.getHeight();

    // Ensure input is grayscale (should have 1 channel)
    if (input.getChannels() == 1) {
    // Step 1: Compute the histogram (256 bins)
        std::vector<int> histogram(256, 0);
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int intensity = input.getPixel(x, y).getR();  // Since grayscale, R=G=B
                histogram[intensity]++;
            }
        }

        // Step 2: Compute the cumulative distribution function (CDF)
        std::vector<int> cdf(256, 0);
        cdf[0] = histogram[0];
        for (int i = 1; i < 256; ++i) {
            cdf[i] = cdf[i - 1] + histogram[i];
        }

        // Step 3: Normalize the CDF (stretch values to [0,255])
        int minCdf = *std::min_element(cdf.begin(), cdf.end());
        int totalPixels = width * height;
        std::vector<unsigned char> equalizationMap(256, 0);

        for (int i = 0; i < 256; ++i) {
            equalizationMap[i] = static_cast<unsigned char>(
                ((cdf[i] - minCdf) * 255.0f) / (totalPixels - minCdf)
            );
        }

        // Step 4: Apply Equalization to the Image
        Image output(width, height, 1);  // Output remains grayscale
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int oldIntensity = input.getPixel(x, y).getR();
                unsigned char newIntensity = equalizationMap[oldIntensity];
                output.setPixel(x, y, Pixel(newIntensity, newIntensity, newIntensity));
            }
        }
        return output;
    }
    else {
        throw std::invalid_argument("Grayscale histogram equalization requires a single-channel image.");
        return input;
    }
}

Image SimpleFilters::applyHistogramEqualization(const Image& input) {
    int width = input.getWidth();
    int height = input.getHeight();
    int channels = input.getChannels();

    // If the image is grayscale, apply standard histogram equalization
    if (channels > 1) {
        // Otherwise, perform histogram equalization on the V channel (HSV)
        Image output(width, height, 3);  // Output is still an RGB image

        std::vector<int> histogram(256, 0);
        std::vector<int> cdf(256, 0);

        // Step 1: Convert RGB to HSV and compute histogram of intensity (V channel)
        std::vector<unsigned char> vValues;
        vValues.reserve(width * height);

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                Pixel pixel = input.getPixel(x, y);
                float h, s, v;
                pixel.RGBtoHSV(h, s, v);

                int intensity = static_cast<int>(v * 255.0f);
                vValues.push_back(intensity);
                histogram[intensity]++;
            }
        }

        // Step 2: Compute cumulative distribution function (CDF)
        cdf[0] = histogram[0];
        for (int i = 1; i < 256; ++i) {
            cdf[i] = cdf[i - 1] + histogram[i];
        }

        // Step 3: Normalize the CDF to scale values between 0-255
        int minCdf = *std::min_element(cdf.begin(), cdf.end());
        int totalPixels = width * height;
        std::vector<unsigned char> equalizationMap(256, 0);

        for (int i = 0; i < 256; ++i) {
            equalizationMap[i] = static_cast<unsigned char>(
                ((cdf[i] - minCdf) * 255.0f) / (totalPixels - minCdf)
            );
        }

        // Step 4: Apply Equalization to the V channel and convert back to RGB
        int index = 0;
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                Pixel pixel = input.getPixel(x, y);
                float h, s, v;
                pixel.RGBtoHSV(h, s, v);

                int oldIntensity = vValues[index++];
                float newV = equalizationMap[oldIntensity] / 255.0f;  // Scale back to [0,1]

                Pixel newPixel;
                newPixel.HSVtoRGB(h, s, newV);
                output.setPixel(x, y, newPixel);
            }
        }

        return output;
    } else {
        return applyGrayscaleHistogramEqualization(input);
    }
}

Image SimpleFilters::applyThresholding(const Image& input, int threshold) {
    Image output = input;
    for (int y = 0; y < input.getHeight(); ++y) {
        for (int x = 0; x < input.getWidth(); ++x) {
            Pixel p = input.getPixel(x, y);
            float intensity = p.getLuminance();
            Pixel newPixel = (intensity < threshold) ? Pixel(0, 0, 0) : Pixel(255, 255, 255);
            output.setPixel(x, y, newPixel);
        }
    }
    return output;
}

Image SimpleFilters::applySaltAndPepperNoise(const Image& input, float noisePercentage) {
    Image output = input;
    int totalPixels = input.getWidth() * input.getHeight();
    int noisePixels = static_cast<int>(totalPixels * (noisePercentage / 100.0f));

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> distX(0, input.getWidth() - 1);
    std::uniform_int_distribution<int> distY(0, input.getHeight() - 1);
    std::bernoulli_distribution binary(0.5);

    for (int i = 0; i < noisePixels; ++i) {
        int x = distX(rng);
        int y = distY(rng);
        Pixel noisePixel = binary(rng) ? Pixel(0, 0, 0) : Pixel(255, 255, 255);
        output.setPixel(x, y, noisePixel);
    }
    return output;
}
