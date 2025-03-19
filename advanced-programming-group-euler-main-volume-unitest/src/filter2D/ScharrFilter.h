/**
 * @file ScharrFilter.h
 * @brief Declaration of the ScharrFilter class for edge detection
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

#ifndef SCHARR_FILTER_H
#define SCHARR_FILTER_H

#include "ConvolutionFilter.h"

/**
 * @class ScharrFilter
 * @brief Filter class for edge detection using the Scharr operator
 * 
 * The ScharrFilter applies two 3x3 kernels to detect horizontal and vertical edges,
 * then combines the results to produce an edge magnitude image.
 * 
 * Kernels used:
 * \n Gx = [[ -3,   0,  3],
 *         [-10,   0, 10],
 *         [ -3,   0,  3]]
 * 
 * \n Gy = [[ -3, -10, -3],
 *         [  0,   0,  0],
 *         [  3,  10,  3]]
 */
class ScharrFilter : public ConvolutionFilter {
public:
    /**
     * @brief Size of the Scharr kernels (3x3)
     */
    static const int KERNEL_SIZE = 3;

private:
    /**
     * @brief 3x3 kernel for horizontal edge detection
     */
    int kernelX[KERNEL_SIZE][KERNEL_SIZE] = {
        { -3,  0,  3},
        {-10,  0, 10},
        { -3,  0,  3}
    };

    /**
     * @brief 3x3 kernel for vertical edge detection
     */
    int kernelY[KERNEL_SIZE][KERNEL_SIZE] = {
        { -3, -10, -3},
        {  0,   0,  0},
        {  3,  10,  3}
    };

public:
    /**
     * @brief Constructor for the ScharrFilter class
     */
    ScharrFilter();

    /**
     * @brief Apply the Scharr edge detection filter to an input image
     * 
     * This method first converts the image to grayscale (if it's RGB),
     * then applies the Scharr operator to detect edges, and normalizes
     * the output.
     * 
     * @param input Input image to apply edge detection to
     * @return Image Edge-detected image after applying the filter
     */
    Image apply(const Image& input) override;
};

#endif // SCHARR_FILTER_H
