/**
 * @file DataContainer.h
 * @brief Declaration of the abstract DataContainer class
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

#ifndef DATA_CONTAINER_H
#define DATA_CONTAINER_H

#include "Pixel.h"

#include <string>
#include <utility> // for std::pair
#include <stdexcept> // for std::invalid_argument
#include <memory> // for std::unique_ptr


/**
 * @brief Abstract base class for image and volume data containers
 * 
 * DataContainer provides a common interface for 2D images and 3D volumes, with shared functionality for handling data coordinates and dimensions.
 * It establishes the polymorphic behavior for derived classes like Image and Volume.
 */
class DataContainer {
protected:
    int width;   ///< Width of the data container
    int height;  ///< Height of the data container
    std::string name; ///< Optional name/identifier for the container

    /**
     * @brief Protected constructor to ensure this class is only used as a base class
     *         Uses validateDimension() to ensure dimensions are positive
     * 
     * @param width Width of the data container
     * @param height Height of the data container
     * @param name Optional name/identifier for the container
     * @throws std::invalid_argument If dimensions are not positive
     */
    DataContainer(int width, int height, const std::string& name = "");

private:
    /**
     * @brief Validate that a dimension is positive
     * 
     * @param value The dimension to validate
     * @return int The validated dimension
     * @throws std::invalid_argument If the dimension is zero or negative
     */
    static int validateDimension(int value);

public:
    /**
     * @brief Virtual destructor to ensure proper cleanup in derived classes
     */
    virtual ~DataContainer() = default;

    /**
     * @brief Pure virtual method to clone the data container
     * This method creates a deep copy of the data container.
     * 
     * @return std::unique_ptr<DataContainer> A unique pointer to a new instance with the same data
     */
    virtual std::unique_ptr<DataContainer> clone() const = 0;

    // *******************************************************************************************
    // -------------------------------------------------------------------------------------------
    // GETTERS & SETTERS
    // -------------------------------------------------------------------------------------------
    // *******************************************************************************************

    /**
     * @brief Get the width of the data container
     * 
     * @return int width of container
     */
    int getWidth() const;

    /**
     * @brief Get the height of the data container
     * 
     * @return int height of container
     */
    int getHeight() const;

    /**
     * @brief Get the dimensions of the data container
     * 
     * @return std::pair<int, int> A pair containing (width, height)
     */
    std::pair<int, int> getDimensions() const;

    /**
     * @brief Get the name/identifier of the container
     * 
     * @return std::string The name
     */
    std::string getName() const;

    /**
     * @brief Set the name/identifier of the container
     * 
     * @param name The new name
     */
    void setName(const std::string& name);

    /**
     * @brief Pure virtual method to get the number of channels
     * 
     * @return int The number of channels (1 for grayscale, 3 for RGB, 4 for RGBA)
     */
    virtual int getChannels() const = 0;

    /**
     * @brief Pure virtual method to get a pixel at a specific position
     * 
     * This method must be implemented by derived classes.
     * 
     * @param x X-coordinate
     * @param y Y-coordinate
     * @return Pixel The pixel at the specified position
     * @throws std::out_of_range If coordinates are out of bounds
     */
    virtual Pixel getPixel(int x, int y) const = 0;

    /**
     * @brief Pure virtual method to set a pixel at a specific position
     * 
     * This method must be implemented by derived classes.
     * 
     * @param x X-coordinate
     * @param y Y-coordinate
     * @param pixel The pixel to assign to that coordinate pair
     * @throws std::out_of_range If coordinates are out of bounds
     */
    virtual void setPixel(int x, int y, const Pixel& pixel) = 0;


    // *******************************************************************************************
    // -------------------------------------------------------------------------------------------
    // LOADING & SAVING
    // -------------------------------------------------------------------------------------------
    // *******************************************************************************************

    /**
     * @brief Pure virtual method to save the data container to a file
     * 
     * @param filename The name of the file to save to
     * @return bool True if the operation was successful, false otherwise
     */
    virtual bool saveToFile(const std::string& filename) const = 0;
    
    // *******************************************************************************************
    // -------------------------------------------------------------------------------------------
    // IMAGE MANIPULATION & OTHER FUNCTIONS
    // -------------------------------------------------------------------------------------------
    // *******************************************************************************************

    /**
     * @brief Apply a filter to the data container (to be implemented by derived classes)
     * This method allows for polymorphic filter application.
     * 
     * @param filterName The name of the filter to apply (eg. "Greyscale", "Brightness", etc.)
     * @param parameters Optional parameters for the filter (e.g. intensity, kernel size)
     * @return std::unique_ptr<DataContainer> A unique pointer to a new data container with the filter applied
     */
    virtual std::unique_ptr<DataContainer> applyFilter(const std::string& filterName, 
                                                    const std::string& parameters = "") const = 0;
    /**
     * @brief Check if provided coordinates are within bounds
     * 
     * @param x X-coordinate
     * @param y Y-coordinate
     * @return true If (x,y) is within the data container bounds
     * @return false if provided coordinates are out of bounds
     */
    bool isInBounds(int x, int y) const;
};    
#endif // DATA_CONTAINER_H