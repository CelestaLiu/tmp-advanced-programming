/**
 * @file RobertsCrossFilter.h
 * @brief Declaration of the RobertsCrossFilter class for edge detection
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

#ifndef ROBERTS_CROSS_FILTER_H
#define ROBERTS_CROSS_FILTER_H

#include "ConvolutionFilter.h"

/**
 * @class RobertsCrossFilter
 * @brief Filter class for edge detection using the Roberts Cross operator
 * 
 * The RobertsCrossFilter applies two 2x2 kernels to detect edges,
 * then combines the results to produce an edge magnitude image.
 * 
 * Kernels used:
 * \n G1 = [[1,  0],
 *         [0, -1]]
 * 
 * \n G2 = [[ 0, 1],
 *         [-1, 0]]
 */
class RobertsCrossFilter : public ConvolutionFilter {
public:
    /**
     * @brief Size of the Roberts Cross kernels (2x2)
     */
    static const int KERNEL_SIZE = 2;

private:
    /**
     * @brief 2x2 kernel for the first Roberts Cross operator
     */
    int kernel1[KERNEL_SIZE][KERNEL_SIZE] = {
        { 1,  0},
        { 0, -1}
    };

    /**
     * @brief 2x2 kernel for the second Roberts Cross operator
     */
    int kernel2[KERNEL_SIZE][KERNEL_SIZE] = {
        { 0,  1},
        {-1,  0}
    };

public:
    /**
     * @brief Constructor for the RobertsCrossFilter class
     */
    RobertsCrossFilter();

    /**
     * @brief Apply the Roberts Cross edge detection filter to an input image
     * 
     * This method first converts the image to grayscale (if it's RGB),
     * then applies the two 2x2 Roberts Cross operators to detect edges,
     * and normalizes the output.
     * 
     * @param input Input image to apply edge detection to
     * @return Image Edge-detected image after applying the filter
     */
    Image apply(const Image& input) override;
};

#endif // ROBERTS_CROSS_FILTER_H
