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

// *******************************************************************************************
// -------------------------------------------------------------------------------------------
// CONSTRUCTORS & DESTRUCTOR
// -------------------------------------------------------------------------------------------
// *******************************************************************************************

SimpleFilters::SimpleFilters(const std::string& filterName) 
    : Filter(filterName) {}

// *******************************************************************************************
// -------------------------------------------------------------------------------------------
// IMAGE MANIPULATION
// -------------------------------------------------------------------------------------------
// *******************************************************************************************

// Apply the selected filter to an image
Image SimpleFilters::apply(const Image& input) {
    if (name == "Greyscale") {
        return applyGreyscale(input);
    }else if (name == "Brightness") {
        if (parameters.find("brightness") != parameters.end()) {
            try {
                int brightness = std::any_cast<int>(parameters["brightness"]);
                if (brightness < -255 || brightness > 255) {
                    throw std::invalid_argument("Brightness value must be between -255 and 255.");
                }
                return applyBrightness(input, brightness);
            } catch (const std::bad_any_cast&) {
                throw std::invalid_argument("Invalid brightness value. Expected an integer.");
            }
        }
        throw std::invalid_argument("Brightness filter requires a brightness value.");
     } else if (name == "HistogramEqualization") {
        return applyHistogramEqualization(input);
    } else if (name == "SaltAndPepperNoise") {
        if (parameters.find("noise") != parameters.end()) {
            try {
                float noise = std::any_cast<float>(parameters["noise"]);
                if (noise < 0.0f || noise > 100.0f) {
                    throw std::invalid_argument("Noise percentage must be between 0 and 100.");
                }
                return applySaltAndPepperNoise(input, noise);
            } catch (const std::bad_any_cast&) {
                throw std::invalid_argument("Invalid noise percentage. Expected a float.");
            }
        }
        throw std::invalid_argument("Salt-and-pepper noise filter requires a noise percentage.");
    }
    else if (name == "Thresholding") {
        if (parameters.find("threshold") != parameters.end()) {
            try {
                int threshold = std::any_cast<int>(parameters["threshold"]);
                if (threshold < 0 || threshold > 255) {
                    throw std::invalid_argument("Threshold value must be between 0 and 255.");
                }
                return applyThresholding(input, threshold);
            } catch (const std::bad_any_cast&) {
                throw std::invalid_argument("Invalid threshold value. Expected an integer.");
            }
        }
        throw std::invalid_argument("Threshold filter requires a threshold value.");
    } 
    else{
        throw std::invalid_argument("Unknown filter: " + name);
    }
}
// Adjust the brightness of an image
Image SimpleFilters::applyBrightness(const Image& input, int brightness) {
    Image output = input;

    // If brightness is 0, normalise image brightness to 128
    if (brightness == 0) {
        long long totalBrightness = 0;
        int totalPixels = input.getWidth() * input.getHeight();

        // Compute the average brightness
        for (int y = 0; y < input.getHeight(); ++y) {
            for (int x = 0; x < input.getWidth(); ++x) {
                totalBrightness += input.getPixel(x, y).getLuminance();
            }
        }
        int avgBrightness = static_cast<int>(totalBrightness / totalPixels);
        brightness = 128 - avgBrightness;  // Adjust brightness to reach 128
    }

    // Apply brightness adjustment
    for (int y = 0; y < input.getHeight(); ++y) {
        for (int x = 0; x < input.getWidth(); ++x) {
            Pixel p = input.getPixel(x, y).adjustBrightness(brightness);
            output.setPixel(x, y, p);
        }
    }

    return output;
}

// Compute the histogram equalization mapping for an image
std::vector<unsigned char> SimpleFilters::computeHistogramEqualizationMap(const std::vector<int>& histogram) {
    std::vector<int> cdf(256, 0);
    cdf[0] = histogram[0];

    // Compute cumulative distribution function
    for (int i = 1; i < 256; ++i) {
        cdf[i] = cdf[i - 1] + histogram[i];
    }

    int minCdf = *std::min_element(cdf.begin(), cdf.end());
    int totalPixels = cdf[255];

    if (totalPixels == minCdf) { 
        return std::vector<unsigned char>(256, 0); // Prevent division by zero
    }

    std::vector<unsigned char> equalizationMap(256, 0);
    for (int i = 0; i < 256; ++i) {
        equalizationMap[i] = static_cast<unsigned char>(
            ((cdf[i] - minCdf) * 255.0f) / (totalPixels - minCdf)
        );
    }
    return equalizationMap;
}

// Convert an RGB image to greyscale
Image SimpleFilters::applyGreyscale(const Image& input) {
    Image greyscale(input.getWidth(), input.getHeight(), 1);
    for (int y = 0; y < input.getHeight(); ++y) {
        for (int x = 0; x < input.getWidth(); ++x) {
            greyscale.setPixel(x, y, input.getPixel(x, y).toGreyscale());
        }
    }
    return greyscale;
}

// Apply Histogram Equalization to a greyscale image
Image SimpleFilters::applyGreyscaleHistogramEqualization(const Image& input) {
    int width = input.getWidth();
    int height = input.getHeight();

    if (input.getChannels() != 1) {
        throw std::invalid_argument("Greyscale histogram equalization requires a single-channel image.");
    }
    // Compute histogram (256 bins)
    std::vector<int> histogram(256, 0);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int intensity = input.getPixel(x, y).getR();
            histogram[intensity]++;
        }
    }
    // Compute equalization map
    std::vector<unsigned char> equalizationMap = computeHistogramEqualizationMap(histogram);

    // Apply equalization
    Image output(width, height, 1);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int oldIntensity = input.getPixel(x, y).getR();
            unsigned char newIntensity = equalizationMap[oldIntensity];
            output.setPixel(x, y, Pixel(newIntensity, newIntensity, newIntensity));
        }
    }

    return output;
}
// Apply Histogram Equalization to an image (calls greyscale version if needed)
Image SimpleFilters::applyHistogramEqualization(const Image& input) {
    int width = input.getWidth();
    int height = input.getHeight();
    int channels = input.getChannels();

    if (channels == 1) {
        return applyGreyscaleHistogramEqualization(input);
    }
    // Process only RGB channels
    Image output(width, height, 3); 
    std::vector<int> histogram(256, 0);
    std::vector<unsigned char> vValues;

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

    std::vector<unsigned char> equalizationMap = computeHistogramEqualizationMap(histogram);

    int index = 0;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Pixel pixel = input.getPixel(x, y);
            float h, s, v;
            pixel.RGBtoHSV(h, s, v);

            int oldIntensity = vValues[index++];
            float newV = equalizationMap[oldIntensity] / 255.0f;

            Pixel newPixel;
            newPixel.HSVtoRGB(h, s, newV);
            output.setPixel(x, y, newPixel);
        }
    }

    return output;
}
// Add salt-and-pepper noise to an image
Image SimpleFilters::applySaltAndPepperNoise(const Image& input, float noisePercentage) {
    if (noisePercentage < 0.0f || noisePercentage > 100.0f) {
        throw std::invalid_argument("Noise percentage must be between 0 and 100.");
    }

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

// Apply thresholding to an image
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


