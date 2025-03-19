/**
 * @file Image.h
 * @brief Declaration of the Image class for image processing
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


#ifndef IMAGE_H
#define IMAGE_H

#include "DataContainer.h"

#include <vector>
#include <string>

/**
 * @brief Class representing a 2D image with pixel data
 * 
 * The Image class extends the DataContainer class to represent a 2D image with pixel data.
 * It provides methods for loading and saving images, as well as applying image processing filters.
 * The pixel data is stored as a 2D array of Pixel objects.
 */
class Image : public DataContainer {
private:
    std::vector<std::vector<Pixel>> pixels;  ///< 2D array of pixels
    int channels;  ///< Number of color channels

public:
    // *******************************************************************************************
    // -------------------------------------------------------------------------------------------
    // CONSTRUCTORS, DESTRUCTOR & CLONE
    // -------------------------------------------------------------------------------------------
    // *******************************************************************************************
    
    /**
     * @brief Construct a new Image object with the specified dimensions
     * 
     * @param width Width of the image
     * @param height Height of the image
     * @param channels Number of color channels (default 3 for RGB)
     * @throws std::invalid_argument If width or height is not positive (inherits from DataContainer)
     * @see DataContainer::validateDimension()
     */
    Image(int width, int height, int channels = 3);

    /**
     * @brief Construct a new Image object by loading from a file
     * 
     * @param filename Path to the image file
     * @throws std::runtime_error If the image file cannot be loaded
     * @details This constructor uses the STB image library to load the image file
     */
    Image(const std::string& filename);

    /**
     * @brief Destructor for the Image object
     * 
     * @details This destructor is virtual to ensure proper cleanup of derived classes
     * @see DataContainer::~DataContainer
     */
    ~Image() override = default;

    /**
     * @brief Create a deep copy of the image
     * 
     * @return std::unique_ptr<DataContainer> A new Image object with the same pixel data
     * @details This method is inherited from the DataContainer class
     * @see DataContainer::clone
     */
    std::unique_ptr<DataContainer> clone() const override;
    
    // *******************************************************************************************
    // -------------------------------------------------------------------------------------------
    // GETTERS & SETTERS
    // -------------------------------------------------------------------------------------------
    // *******************************************************************************************

    // Override abstract methods from DataContainer
    /**
     * @brief Get the pixel at the specified position
     * 
     * @param x X-coordinate
     * @param y Y-coordinate
     * @return Pixel The pixel at the specified position
     * @throws std::out_of_range If the coordinates are out of bounds
     * @details This method is inherited from the DataContainer class
     * @see DataContainer::getPixel
     */   
    Pixel getPixel(int x, int y) const override;

    /**
     * @brief Set the pixel at the specified position
     * 
     * @param x X-coordinate
     * @param y Y-coordinate
     * @param pixel The pixel to set
     * @throws std::out_of_range If the coordinates are out of bounds
     * @details This method is inherited from the DataContainer class
     * @see DataContainer::setPixel
     */ 
    void setPixel(int x, int y, const Pixel& pixel) override;
    
    /**
     * @brief Get the number of color channels
     * 
     * @return int The number of color channels
     * @details This method is inherited from the DataContainer class
     * @see DataContainer::getChannels
     */
    int getChannels() const override;
   
    // *******************************************************************************************
    // -------------------------------------------------------------------------------------------
    // LOADING & SAVING
    // -------------------------------------------------------------------------------------------
    // *******************************************************************************************

    /**
     * @brief Save the image to a file
     * 
     * @param filename Path to the output file
     * @return true If the image was saved successfully
     * @details This method uses the STB image library to save the image
     */
    bool saveToFile(const std::string& filename) const override;

    /**
     * @brief Load an image from a file
     * 
     * @param filename Path to the image file
     * @return true If the image was loaded successfully
     * @throws std::runtime_error If the image file cannot be loaded
     * @details This method uses the STB image library to load the image
     * @see Image::Image(const std::string& filename)
     */
    bool loadFromFile(const std::string& filename);

    // *******************************************************************************************
    // -------------------------------------------------------------------------------------------
    // IMAGE MANIPULATION
    // -------------------------------------------------------------------------------------------
    // *******************************************************************************************

    /**
     * @brief Apply a filter to the image
     * 
     * @param filterName Name of the filter to apply
     * @param parameters Additional parameters for the filter (optional)
     * @return std::unique_ptr<DataContainer> A new Image object with the filter applied
     * @throws std::invalid_argument If a non-integer parameter is provided where an integer is expected
     * @throws std::runtime_error If an unknown filter is requested
     * @details This method is inherited from the DataContainer class
     * @see DataContainer::applyFilter
     */
    std::unique_ptr<DataContainer> applyFilter(const std::string& filterName, 
        const std::string& parameters = "") const override;

    /**
     * @brief Convert the image to greyscale
     * 
     * @return Image A new Image object with single greyscale channel & pixel values
     * @details This method converts the image to greyscale by averaging the RGB values and setting all color channels to the same value. 
     * If the image is already greyscale, it returns a copy of the original image.
     * @see Pixel::toGreyscale
     */
    Image toGreyscale() const;
};

#endif // IMAGE_H