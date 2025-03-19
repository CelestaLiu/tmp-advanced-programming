#ifndef MEDIAN_BLUR_FILTER_H
#define MEDIAN_BLUR_FILTER_H

#include "Filter.h"
#include "../Image.h"

#include <stdexcept>
#include <vector>
#include <algorithm>

/**
 * @file MedianBlurFilter.h
 * @brief Declaration of the MedianBlurFilter class that applies a median blur to an image.
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

/**
 * @brief Median blur filter that replaces each pixel with the median value of its neighbors.
 * 
 * The MedianBlurFilter collects the pixel values within a defined neighborhood around each pixel,
 * sorts the values for each color channel individually, and then selects the median. 
 */
class MedianBlurFilter : public Filter {
public:
    // *******************************************************************************************
    // -------------------------------------------------------------------------------------------
    // CONSTRUCTORS
    // -------------------------------------------------------------------------------------------
    // *******************************************************************************************

    /**
     * @brief Constructs a new MedianBlurFilter object.
     * 
     * The kernel size must be an odd integer and at least 3 to ensure proper functionality.
     * If an invalid kernel size is provided, an std::invalid_argument exception is thrown.
     * 
     * @param kernelSize Size of the kernel (must be odd and >= 3).
     * @throws std::invalid_argument if kernelSize is even or less than 3.
     */
    explicit MedianBlurFilter(int kernelSize);
    // *******************************************************************************************
    // -------------------------------------------------------------------------------------------
    // APPLY FUNCTION
    // -------------------------------------------------------------------------------------------
    // *******************************************************************************************
    /**
     * @brief Applies the median blur filter to the input image.
     * 
     * For each pixel in the input image, the filter gathers neighboring pixel values within the 
     * kernel, sorts each channel separately, and uses the median value to form the output pixel.
     * 
     * @param input The input image to process.
     * @return Image The resulting blurred image.
     */
    Image apply(const Image& input) override;

private:
    int kernelSize;
};

#endif // MEDIAN_BLUR_FILTER_H


