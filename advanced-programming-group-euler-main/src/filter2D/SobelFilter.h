/**
 * @file SobelFilter.h
 * @brief Declaration of the SobelFilter class for edge detection
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

 #ifndef SOBEL_FILTER_H
 #define SOBEL_FILTER_H
 
 #include "ConvolutionFilter.h"
 #include <algorithm>
#include <cmath>
#include <vector>
 
 /**
  * @brief Filter class for edge detection using the Sobel operator
  * 
  * The SobelFilter applies two 3x3 kernels to detect horizontal and vertical edges,
  * and then combines the results to produce an edge magnitude image.
  * The kernels used are:
  * 
  * Gx = [[-1, 0, 1],
  *       [-2, 0, 2],
  *       [-1, 0, 1]]
  * 
  * Gy = [[-1, -2, -1],
  *       [ 0,  0,  0],
  *       [ 1,  2,  1]]
  */
 class SobelFilter : public ConvolutionFilter {
 private:
     // Sobel kernels for edge detection
     static const int KERNEL_SIZE = 3;
     
     // Kernel for horizontal edge detection
     int kernelX[KERNEL_SIZE][KERNEL_SIZE] = {
         {-1, 0, 1},
         {-2, 0, 2},
         {-1, 0, 1}
     };
     
     // Kernel for vertical edge detection
     int kernelY[KERNEL_SIZE][KERNEL_SIZE] = {
         {-1, -2, -1},
         { 0,  0,  0},
         { 1,  2,  1}
     };
 
 public:
     /**
      * @brief Constructor for the SobelFilter class
      */
     SobelFilter();
     
     /**
      * @brief Apply the Sobel edge detection filter to an input image
      * 
      * This method first converts the image to greyscale (if it's RGB),
      * then applies the Sobel operator to detect edges, and normalizes
      * the output.
      * 
      * @param input Input image to apply edge detection to
      * @return Image Edge-detected image after applying the filter
      */
     Image apply(const Image& input) override;
 };
 
 #endif // SOBEL_FILTER_H