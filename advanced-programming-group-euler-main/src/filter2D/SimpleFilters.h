/**
 * @file SimpleFilters.h
 * @brief Implementation of basic image processing filters.
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


 #ifndef SIMPLE_FILTERS_H
 #define SIMPLE_FILTERS_H
 
 #include "Filter.h"
 
#include <algorithm>  // For min/max operations
#include <random>     // For noise generation
#include <vector>     // For vector operations
 
 /**
  * @brief SimpleFilters class implementing basic per-pixel image filters.
  * 
  * This class inherits from `Filter` and implements a variety of simple 
  * image processing techniques like greyscale conversion, brightness adjustment, 
  * histogram equalization, thresholding, and noise addition.
  */
 class SimpleFilters : public Filter {
 public:
    // *******************************************************************************************
    // -------------------------------------------------------------------------------------------
    // CONSTRUCTORS, DESTRUCTOR & CLONE
    // -------------------------------------------------------------------------------------------
    // *******************************************************************************************
    /**
      * @brief Constructor for the SimpleFilters class.
      * 
      * @param filterName The name of the filter.
    */
     explicit SimpleFilters(const std::string& filterName);

    // *******************************************************************************************
    // -------------------------------------------------------------------------------------------
    // IMAGE MANIPULATION
    // -------------------------------------------------------------------------------------------
    // *******************************************************************************************

     /**
      * @brief Apply the selected filter to an image.
      * This function determines which filter to apply based on `filterName`.
      * 
      * @param input The input image.
      * @return Image The processed image.
      */
     Image apply(const Image& input) override;
     
     /**
      * @brief Adjust the brightness of an image.
      * 
      * Adds a user-defined brightness value (-255 to 255) to all pixels,
      * or normalises the average brightness to 128.
      * 
      * @param input The input image.
      * @param brightness The brightness adjustment value.
      * @return Image The adjusted image.
      */
     Image applyBrightness(const Image& input, int brightness = 0);

    /**
     * @brief Compute the histogram equalization mapping for an image.
     * @param histogram Histogram of pixel intensities.
     * @return Vector mapping old intensities to new ones.
     */
    static std::vector<unsigned char> computeHistogramEqualizationMap(const std::vector<int>& histogram);

    /**
      * @brief Convert an image to greyscale.
      * 
      * Reduces the image to a single greyscale channel using the formula:
      * \f$ Y = 0.2126R + 0.7152G + 0.0722B \f$
      * 
      * @param input The input image.
      * @return Image The greyscale image.
      */
    Image applyGreyscale(const Image& input);
 
    /**
    * @brief Perform histogram equalization on a greyscale image.
    * Stretches the intensity histogram to fill the range 0-255.
    * 
    * @param input The input image.
    * @return Image The equalised image.
    */
    Image applyGreyscaleHistogramEqualization(const Image& input);

     /**
      * @brief Perform histogram equalization.
      * Stretches the intensity histogram to fill the range 0-255.
      * Calls the greyscale version if needed.
      * 
      * @param input The input image.
      * @return Image The equalised image.
      */
     Image applyHistogramEqualization(const Image& input);


    /**
      * @brief Add salt-and-pepper noise to an image.
      * Randomly converts a percentage of pixels to black or white.
      * 
      * @param input The input image.
      * @param noisePercentage The percentage of pixels affected (0-100%)
      * @return Image The noisy image.
      * 
      * @note The noise percentage is the percentage of pixels affected by noise.
      * @throws std::invalid_argument If noisePercentage is not in the range [0, 100].
    */
    Image applySaltAndPepperNoise(const Image& input, float noisePercentage);

     /**
      * @brief Apply thresholding to an image.
      * Converts all pixels below a given intensity to black and others to white.
      * 
      * @param input The input image.
      * @param threshold The threshold value (0-255).
      * @return Image The thresholded image.
      */
     Image applyThresholding(const Image& input, int threshold);
 
 };
 
 #endif // SIMPLE_FILTERS_H
 