/**
 * @file PrewittFilter.h
 * @brief Declaration of the PrewittFilter class for edge detection
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

#ifndef PREWITT_FILTER_H
#define PREWITT_FILTER_H

#include "ConvolutionFilter.h"

/**
 * @class PrewittFilter
 * @brief Filter class for edge detection using the Prewitt operator
 * 
 * The PrewittFilter applies two 3x3 kernels to detect horizontal and vertical edges,
 * then combines the results to produce an edge magnitude image.
 * 
 * Kernels used:
 * \n Gx = [[-1, 0, 1],
 *         [-1, 0, 1],
 *         [-1, 0, 1]]
 * 
 * \n Gy = [[-1, -1, -1],
 *         [ 0,  0,  0],
 *         [ 1,  1,  1]]
 */
class PrewittFilter : public ConvolutionFilter {
public:
    /**
     * @brief Size of the Prewitt kernels (3x3)
     */
    static const int KERNEL_SIZE = 3;

private:
    /**
     * @brief 3x3 kernel for horizontal edge detection
     */
    int kernelX[KERNEL_SIZE][KERNEL_SIZE] = {
        {-1, 0,  1},
        {-1, 0,  1},
        {-1, 0,  1}
    };

    /**
     * @brief 3x3 kernel for vertical edge detection
     */
    int kernelY[KERNEL_SIZE][KERNEL_SIZE] = {
        {-1, -1, -1},
        { 0,  0,  0},
        { 1,  1,  1}
    };

public:
    /**
     * @brief Constructor for the PrewittFilter class
     */
    PrewittFilter();

    /**
     * @brief Apply the Prewitt edge detection filter to an input image
     * 
     * This method first converts the image to grayscale (if it's RGB),
     * then applies the Prewitt operator to detect edges, and normalizes
     * the output.
     * 
     * @param input Input image to apply edge detection to
     * @return Image Edge-detected image after applying the filter
     */
    Image apply(const Image& input) override;
};

#endif // PREWITT_FILTER_H
