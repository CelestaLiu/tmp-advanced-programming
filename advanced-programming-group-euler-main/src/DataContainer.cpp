/**
 * @file DataContainer.cpp
 * @brief Implementation/Definition of the abstract DataContainer class
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

#include "DataContainer.h"

// *******************************************************************************************
// -------------------------------------------------------------------------------------------
// CONSTRUCTORS, DESTRUCTOR & CLONE
// -------------------------------------------------------------------------------------------
// *******************************************************************************************

// Constructor with dimensions
DataContainer::DataContainer(int width, int height, const std::string& name)
    : width(validateDimension(width)),  // Call validateDimension() before assignment
      height(validateDimension(height)),
      name(name) {}

// *******************************************************************************************
// -------------------------------------------------------------------------------------------
// GETTERS & SETTERS
// -------------------------------------------------------------------------------------------
// *******************************************************************************************

// Get width
int DataContainer::getWidth() const {
    return width;
}
// Get height
int DataContainer::getHeight() const {
    return height;
}
// Get dimensions
std::pair<int, int> DataContainer::getDimensions() const {
    return std::make_pair(width, height);
}
// Getter for name
std::string DataContainer::getName() const {
    return name;
}
// Setter for name
void DataContainer::setName(const std::string& name) {
    this->name = name;
}

// *******************************************************************************************
// -------------------------------------------------------------------------------------------
// IMAGE MANIPULATION & OTHER FUNCTIONS
// -------------------------------------------------------------------------------------------
// *******************************************************************************************

// Check if coordinates are within bounds
bool DataContainer::isInBounds(int x, int y) const {
    return ((x >= 0) && (x < width) && (y >= 0) && (y < height));
}
// Validate that a dimension is positive (used with constructor)
int DataContainer::validateDimension(int value){
    if (value <= 0){
        throw std::invalid_argument("DataContainer dimensions must be positive");
    }
    return value;
}