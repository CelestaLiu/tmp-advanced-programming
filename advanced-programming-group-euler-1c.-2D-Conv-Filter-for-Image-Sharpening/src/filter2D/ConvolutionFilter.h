/**
 * @file ConvolutionFilter.h
 * @brief Declaration of the ConvolutionFilter class for convolution-based image processing
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

#ifndef CONVOLUTION_FILTER_H
#define CONVOLUTION_FILTER_H

#include "Filter.h"
#include "../Image.h"

#include <vector>
#include <algorithm>
#include <cmath>

/**
 * @class ConvolutionFilter
 * @brief An abstract base class for convolution-based filters
 * 
 * The ConvolutionFilter class extends the Filter class, providing a common interface 
 * for applying 2D convolution kernels to greyscale images. Derived classes can utilize 
 * applyConvolution() to perform kernel-based transformations such as edge detection 
 * or smoothing.
 */
class ConvolutionFilter : public Filter {
protected:
    int kernelSize; ///< The size of the convolution kernel (assumed to be square)

public:
    // *******************************************************************************************
    // -------------------------------------------------------------------------------------------
    // CONSTRUCTORS & DESTRUCTOR
    // -------------------------------------------------------------------------------------------
    // *******************************************************************************************

    /**
     * @brief Constructor for the ConvolutionFilter class
     * 
     * @param filterName A string representing the name of the filter
     * @param kSize The size of the convolution kernel (e.g., 3 for a 3x3 kernel)
     */
    ConvolutionFilter(const std::string& filterName, int kSize);

    /**
     * @brief Virtual destructor for the ConvolutionFilter class
     */
    virtual ~ConvolutionFilter();

    // *******************************************************************************************
    // -------------------------------------------------------------------------------------------
    // CONVOLUTION FUNCTION
    // -------------------------------------------------------------------------------------------
    // *******************************************************************************************

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
    Image applyConvolution(const Image& input, const std::vector<std::vector<int>>& kernel);
};

#endif // CONVOLUTION_FILTER_H
