/**
 * @file BoxBlurFilter.cpp
 * @brief Implementation of the BoxBlurFilter class, which applies a uniform (box) blur to an image.
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

#include "BoxBlurFilter.h"

// *******************************************************************************************
// -------------------------------------------------------------------------------------------
// CONSTRUCTORS & DESTRUCTOR
// -------------------------------------------------------------------------------------------
// *******************************************************************************************
/**
 * @brief Constructs a new BoxBlurFilter object.
 * 
 * The kernel size must be odd and at least 3. If the provided kernel size does not meet 
 * these requirements, an std::invalid_argument exception is thrown.
 * 
 * @param kernelSize Size of the convolution kernel.
 * @throws std::invalid_argument if kernelSize is even or less than 3.
 */
BoxBlurFilter::BoxBlurFilter(int kernelSize)
    : Filter("Box Blur"), kernelSize(kernelSize) {
    if (kernelSize % 2 == 0 || kernelSize < 3) {
        throw std::invalid_argument("Kernel size must be odd and at least 3");
    }
}

// *******************************************************************************************
// -------------------------------------------------------------------------------------------
// APPLY FUNCTION (BOX BLUR IMPLEMENTATION)
// -------------------------------------------------------------------------------------------
// *******************************************************************************************

/**
 * @brief Applies the box blur filter to the input image.
 * 
 * For every pixel in the input image, the method computes the average of the pixel values
 * within the defined neighborhood (kernel) and sets the resulting average as the new pixel 
 * value in the output image. The algorithm ensures that edge pixels are handled by considering 
 * only those neighbors that fall within the image bounds.
 * 
 * @param input The input image to be processed.
 * @return Image The blurred output image.
 */
Image BoxBlurFilter::apply(const Image& input) {
    int width = input.getWidth();
    int height = input.getHeight();
    int channels = input.getChannels();
    Image output(width, height, channels);

    int half = kernelSize / 2;
    // Loop over every pixel in the image
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            double sumR = 0, sumG = 0, sumB = 0, sumA = 0;
            int count = 0;
            // Iterate over the kernel neighborhood
            for (int j = -half; j <= half; j++) {
                for (int i = -half; i <= half; i++) {
                    int nx = x + i;
                    int ny = y + j;
                    // Check if within image bounds
                    if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                        Pixel p = input.getPixel(nx, ny);
                        sumR += p.getR();
                        sumG += p.getG();
                        sumB += p.getB();
                        sumA += p.getA();
                        count++;
                    }
                }
            }
            // Compute average values and set the pixel in the output image
            unsigned char r = static_cast<unsigned char>(sumR / count);
            unsigned char g = static_cast<unsigned char>(sumG / count);
            unsigned char b = static_cast<unsigned char>(sumB / count);
            unsigned char a = static_cast<unsigned char>(sumA / count);
            output.setPixel(x, y, Pixel(r, g, b, a));
        }
    }
    return output;
}

