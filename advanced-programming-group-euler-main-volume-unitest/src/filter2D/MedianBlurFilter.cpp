/**
 * @file MedianBlurFilter.cpp
 * @brief Implementation of the MedianBlurFilter class that applies a median blur to an image.
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

 #include "MedianBlurFilter.h"

// *******************************************************************************************
// -------------------------------------------------------------------------------------------
// CONSTRUCTORS
// -------------------------------------------------------------------------------------------
// *******************************************************************************************

/**
 * @brief Constructs a new MedianBlurFilter object.
 * 
 * The kernel size determines the neighborhood of pixels used to compute the median. It must be
 * an odd integer and at least 3, otherwise an std::invalid_argument exception is thrown.
 * 
 * @param kernelSize Size of the kernel (must be odd and >= 3).
 * @throws std::invalid_argument if kernelSize is even or less than 3.
 */

MedianBlurFilter::MedianBlurFilter(int kernelSize)
    : Filter("Median Blur"), kernelSize(kernelSize) {
    if (kernelSize % 2 == 0 || kernelSize < 3) {
        throw std::invalid_argument("Kernel size must be odd and at least 3");
    }
}
// *******************************************************************************************
// -------------------------------------------------------------------------------------------
// APPLY FUNCTION (MEDIAN BLUR IMPLEMENTATION)
// -------------------------------------------------------------------------------------------
// *******************************************************************************************

/**
 * @brief Applies the median blur filter to the input image.
 * 
 * For each pixel in the input image, the method gathers the pixel values from the neighborhood
 * defined by the kernel. It then sorts the values of each color channel separately and selects
 * the median value.
 * 
 * @param input The input image to process.
 * @return Image The resulting blurred image.
 */
Image MedianBlurFilter::apply(const Image& input) {
    int width = input.getWidth();
    int height = input.getHeight();
    int channels = input.getChannels();
    Image output(width, height, channels);

    int half = kernelSize / 2;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Vectors to store neighbor channel values
            std::vector<unsigned char> reds, greens, blues, alphas;
            for (int j = -half; j <= half; j++) {
                for (int i = -half; i <= half; i++) {
                    int nx = x + i;
                    int ny = y + j;
                    if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                        Pixel p = input.getPixel(nx, ny);
                        reds.push_back(p.getR());
                        greens.push_back(p.getG());
                        blues.push_back(p.getB());
                        alphas.push_back(p.getA());
                    }
                }
            }
            // Sort each vector and take the median value
            std::sort(reds.begin(), reds.end());
            std::sort(greens.begin(), greens.end());
            std::sort(blues.begin(), blues.end());
            std::sort(alphas.begin(), alphas.end());
            
            size_t medianIndex = reds.size() / 2;
            output.setPixel(x, y, Pixel(
                reds[medianIndex],
                greens[medianIndex],
                blues[medianIndex],
                alphas[medianIndex]
            ));
        }
    }
    return output;
}

