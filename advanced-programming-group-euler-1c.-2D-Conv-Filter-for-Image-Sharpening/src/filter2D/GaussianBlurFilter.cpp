/**
 * @file GaussianBlurFilter.cpp
 * @brief Implementation of the GaussianBlurFilter class that applies a Gaussian blur to an image.
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

#include "GaussianBlurFilter.h"


 // *******************************************************************************************
// -------------------------------------------------------------------------------------------
// CONSTRUCTORS
// -------------------------------------------------------------------------------------------
// *******************************************************************************************

/**
 * @brief Constructs a new GaussianBlurFilter object.
 * 
 * Initializes the filter with the specified kernel size and sigma (standard deviation).
 * The kernel size must be odd and at least 3, otherwise an std::invalid_argument exception
 * is thrown.
 * 
 * @param kernelSize Size of the convolution kernel.
 * @param sigma Standard deviation for the Gaussian distribution.
 * @throws std::invalid_argument if kernelSize is even or less than 3.
 */
GaussianBlurFilter::GaussianBlurFilter(int kernelSize, float sigma)
    : Filter("Gaussian Blur"), kernelSize(kernelSize), sigma(sigma) {
    if (kernelSize % 2 == 0 || kernelSize < 3) {
        throw std::invalid_argument("Kernel size must be odd and at least 3");
    }
}

std::vector<std::vector<double>> GaussianBlurFilter::createKernel() {
    std::vector<std::vector<double>> kernel(kernelSize, std::vector<double>(kernelSize, 0.0));
    int half = kernelSize / 2;
    double sum = 0.0;
    double s = 2.0 * sigma * sigma;

    for (int y = -half; y <= half; y++) {
        for (int x = -half; x <= half; x++) {
            double value = exp(-(x*x + y*y) / s) / (M_PI * s);
            kernel[y+half][x+half] = value;
            sum += value;
        }
    }
    // Normalize the kernel
    for (int i = 0; i < kernelSize; i++) {
        for (int j = 0; j < kernelSize; j++) {
            kernel[i][j] /= sum;
        }
    }
    return kernel;
}
// *******************************************************************************************
// -------------------------------------------------------------------------------------------
// APPLY FUNCTION (GAUSSIAN BLUR IMPLEMENTATION)
// -------------------------------------------------------------------------------------------
// *******************************************************************************************

/**
 * @brief Applies the Gaussian blur filter to the input image.
 * 
 * Each pixel processed by computing a weighted sum of its neighbors (weights defined by Gaussian Kernel)
 * 
 * @param input The input image to process.
 * @return Image The resulting blurred image.
 */
Image GaussianBlurFilter::apply(const Image& input) {
    int width = input.getWidth();
    int height = input.getHeight();
    int channels = input.getChannels();
    Image output(width, height, channels);

    auto kernel = createKernel();
    int half = kernelSize / 2;
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            double sumR = 0, sumG = 0, sumB = 0, sumA = 0;
            for (int j = -half; j <= half; j++) {
                for (int i = -half; i <= half; i++) {
                    int nx = x + i;
                    int ny = y + j;
                    if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                        Pixel p = input.getPixel(nx, ny);
                        double weight = kernel[j+half][i+half];
                        sumR += p.getR() * weight;
                        sumG += p.getG() * weight;
                        sumB += p.getB() * weight;
                        sumA += p.getA() * weight;
                    }
                }
            }
            output.setPixel(x, y, Pixel(
                static_cast<unsigned char>(sumR),
                static_cast<unsigned char>(sumG),
                static_cast<unsigned char>(sumB),
                static_cast<unsigned char>(sumA)
            ));
        }
    }
    return output;
}


