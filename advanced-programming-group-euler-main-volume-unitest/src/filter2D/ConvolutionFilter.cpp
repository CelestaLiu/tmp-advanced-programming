/**
 * @file ConvolutionFilter.cpp
 * @brief Implementation of the ConvolutionFilter class
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

#include "ConvolutionFilter.h"

/**
 * @brief Constructor for the ConvolutionFilter class
 * @param filterName A string representing the name of the filter
 * @param kSize The size of the convolution kernel (e.g., 3 for a 3x3 kernel)
 */
ConvolutionFilter::ConvolutionFilter(const std::string& filterName, int kSize)
    : Filter(filterName), kernelSize(kSize) {}

/**
 * @brief Virtual destructor for the ConvolutionFilter class
 */
ConvolutionFilter::~ConvolutionFilter() = default;

/**
 * @brief Utility function to apply a 2D convolution kernel to a greyscale image
 * 
 * This method performs convolution on a greyscale image using the specified kernel 
 * and normalizes the resulting values to the range [0, 255].
 * 
 * @param input The input greyscale Image on which convolution will be performed
 * @param kernel A 2D convolution kernel represented by a std::vector<std::vector<int>>
 * @return Image A new Image object containing the convolution results (greyscale)
 */
Image ConvolutionFilter::applyConvolution(const Image& input, const std::vector<std::vector<int>>& kernel) {
    int width = input.getWidth();
    int height = input.getHeight();
    Image output(width, height, 1);  // Output as a greyscale image

    // Calculate kernel offset (assumes square kernel)
    int kOffset = kernel.size() / 2;
    int maxVal = 0;

    // Temporary storage for convolution results
    std::vector<std::vector<int>> convResult(height, std::vector<int>(width, 0));

    // Perform convolution
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int sum = 0;
            for (int ky = 0; ky < static_cast<int>(kernel.size()); ++ky) {
                for (int kx = 0; kx < static_cast<int>(kernel[ky].size()); ++kx) {
                    // Compute neighbor coordinates
                    int ix = x + kx - kOffset;
                    int iy = y + ky - kOffset;

                    // Boundary handling: clamp to valid range
                    ix = std::max(0, std::min(width - 1, ix));
                    iy = std::max(0, std::min(height - 1, iy));

                    // Get the pixel intensity (assuming greyscale)
                    unsigned char pixel = input.getPixel(ix, iy).getR();
                    sum += pixel * kernel[ky][kx];
                }
            }
            convResult[y][x] = sum;
            if (std::abs(sum) > maxVal) {
                maxVal = std::abs(sum);
            }
        }
    }

    // Normalize results to [0, 255]
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            unsigned char normVal = 0;
            if (maxVal > 0) {
                normVal = static_cast<unsigned char>((std::abs(convResult[y][x]) * 255) / maxVal);
            }
            Pixel p(normVal, normVal, normVal);
            output.setPixel(x, y, p);
        }
    }

    return output;
}
