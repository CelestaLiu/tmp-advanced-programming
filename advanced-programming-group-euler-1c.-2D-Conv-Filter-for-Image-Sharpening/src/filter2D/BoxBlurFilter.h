/**
 * @file BoxBlurFilter.h
 * @brief Declaration of the BoxBlurFilter class that applies a uniform (box) blur to an image.
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

#ifndef BOX_BLUR_FILTER_H
#define BOX_BLUR_FILTER_H

#include "Filter.h"
#include "../Image.h"

#include <stdexcept>


/**
 * @brief Box blur filter that replaces each pixel with the average of its neighbors.
 */
class BoxBlurFilter : public Filter {
public:
    // *******************************************************************************************
    // -------------------------------------------------------------------------------------------
    // CONSTRUCTORS
    // -------------------------------------------------------------------------------------------
    // *******************************************************************************************
    /**
     * @brief Construct a new BoxBlurFilter object.
     * 
     * @param kernelSize Size of the convolution kernel (must be odd and >= 3).
     * @throws std::invalid_argument If kernelSize is even or less than 3.
     */
    explicit BoxBlurFilter(int kernelSize);
    // *******************************************************************************************
    // -------------------------------------------------------------------------------------------
    // APPLY FUNCTION
    // -------------------------------------------------------------------------------------------
    // *******************************************************************************************


    /**
     * @brief Apply the box blur filter on the input image.
     * 
     * @param input The input image to process.
     * @return Image The resulting blurred image.
     */
    Image apply(const Image& input) override;

private:
    int kernelSize;
};

#endif // BOX_BLUR_FILTER_H

