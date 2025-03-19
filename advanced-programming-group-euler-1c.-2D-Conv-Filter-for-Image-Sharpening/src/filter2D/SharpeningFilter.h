/**
 * @file SharpeningFilter.h
 * @brief Declaration of the SharpeningFilter class for image sharpening
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

 #ifndef SHARPENING_FILTER_H
 #define SHARPENING_FILTER_H
 
 #include "Filter.h"
 #include "../Image.h"  
 #include "../Pixel.h" 
 
 #include <algorithm> 
 #include <cmath>     
 
 /**
  * @brief Filter class for sharpening images using a Laplacian kernel
  * 
  * The SharpeningFilter applies a 3x3 Laplacian kernel to detect edges, 
  * and then adds the result back to the original image to enhance edges.
  * The kernel used is:
  * [0, -1, 0]
  * [-1, 4, -1]
  * [0, -1, 0]
  */
 class SharpeningFilter : public Filter {
 private:
     // Laplacian kernel for edge detection
     static const int KERNEL_SIZE = 3;
     int kernel[KERNEL_SIZE][KERNEL_SIZE] = {
         {0, -1, 0},
         {-1, 4, -1},
         {0, -1, 0}
     };
 
 public:
     /**
      * @brief Constructor for the SharpeningFilter class
      */
     SharpeningFilter();
 
     /**
      * @brief Apply the sharpening filter to an input image
      * 
      * @param input Input image to sharpen
      * @return Image Sharpened image after applying the filter
      */
     Image apply(const Image& input) override;
 };
 
 #endif // SHARPENING_FILTER_H