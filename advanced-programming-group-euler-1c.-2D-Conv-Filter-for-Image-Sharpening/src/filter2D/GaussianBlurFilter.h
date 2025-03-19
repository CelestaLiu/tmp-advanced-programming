#ifndef GAUSSIAN_BLUR_FILTER_H
#define GAUSSIAN_BLUR_FILTER_H

#include "Filter.h"
#include "../Image.h"

#include <vector>
#include <stdexcept>
#include <cmath>

/**
 * @file GaussianBlurFilter.h
 * @brief Declaration of the GaussianBlurFilter class that applies a weighted Gaussian blur to an image.
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
 * @brief Gaussian blur filter that replaces each pixel with a weighted average of its neighbors.
 */
class GaussianBlurFilter : public Filter {
public:
    // *******************************************************************************************
    // -------------------------------------------------------------------------------------------
    // CONSTRUCTORS
    // -------------------------------------------------------------------------------------------
    // *******************************************************************************************
    /**
     * @brief Construct a new GaussianBlurFilter object.
     * 
     * @param kernelSize Size of the convolution kernel (must be odd and >= 3).
     * @param sigma Standard deviation for the Gaussian distribution (default 2.0).
     * @throws std::invalid_argument If kernelSize is even or less than 3.
     */
    explicit GaussianBlurFilter(int kernelSize, float sigma = 2.0f);
    // *******************************************************************************************
    // -------------------------------------------------------------------------------------------
    // APPLY FUNCTION
    // -------------------------------------------------------------------------------------------
    // *******************************************************************************************
    /**
     * @brief Apply the Gaussian blur filter on the input image.
     * 
     * @param input The input image to process.
     * @return Image The resulting blurred image.
     */
    Image apply(const Image& input) override;

private:
    int kernelSize;
    float sigma;
    std::vector<std::vector<double>> createKernel();
};

#endif // GAUSSIAN_BLUR_FILTER_H


