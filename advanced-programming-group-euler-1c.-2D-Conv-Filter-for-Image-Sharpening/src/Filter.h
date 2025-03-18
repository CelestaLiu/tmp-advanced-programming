/**
 * @file Filter.h
 * @brief Declaration of the abstract Filter class for image processing
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

#ifndef FILTER_H
#define FILTER_H

#include "Image.h"
#include <string>
#include <map>
#include <any>

/** 
* @brief Abstract base class for image processing filters
* 
* The `Filter` class provides a polymorphic interface for applying image filters.
* It allows for customizable parameters and ensures derived classes implement the 
* `apply()` function for image processing.
*/
class Filter {
protected:
    std::string name; ///< Name of the filter
    std::map<std::string, std::any> parameters; ///< Parameters for filter customization

public:
    // *******************************************************************************************
    // -------------------------------------------------------------------------------------------
    // CONSTRUCTORS, DESTRUCTOR & CLONE
    // -------------------------------------------------------------------------------------------
    // *******************************************************************************************
    
    /**
     * @brief Constructor for the Filter class
     * 
     * @param filterName Name of the filter
     */
    explicit Filter(const std::string& filterName);

    /**
     * @brief Virtual destructor for the Filter class
     */
    virtual ~Filter();

    // *******************************************************************************************
    // -------------------------------------------------------------------------------------------
    // GETTERS & SETTERS
    // -------------------------------------------------------------------------------------------
    // *******************************************************************************************
    
    /**
     * @brief Get the name of the filter
     * 
     * @return std::string Name of the filter
     */
    std::string getName() const;

    /**
     * @brief Set a parameter for the filter
     * 
     * @param key Parameter key
     * @param value Parameter value
     */
    void setParameter(const std::string& key, const std::any& value);

    // *******************************************************************************************
    // -------------------------------------------------------------------------------------------
    // IMAGE MANIPULATION
    // -------------------------------------------------------------------------------------------
    // *******************************************************************************************

    /**
     * @brief Apply the filter to an input image
     * 
     * @param input Input image to apply the filter to
     * @return Image Processed image after applying the filter
     * @details This method must be implemented by derived classes
     * @see Image
     */
    virtual Image apply(const Image& input) = 0;
};

#endif