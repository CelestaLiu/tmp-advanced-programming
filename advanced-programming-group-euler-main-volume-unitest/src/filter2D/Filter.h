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

#include "../Image.h"

#include <string>
#include <map>
#include <any>
#include <variant>

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
    // using std::variant instead of std::any to improve efficiency and type safety
    std::map<std::string, std::variant<int, double, std::string>> parameters; ///< Parameters for filter customization

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
     * stores a key-value pair for the filter parameters
     * 
     * @param key Parameter name (e.g. "threshold")
     * @param value Parameter value (supports 'any' type)
     * @details This method allows for customizing filter behavior by setting parameters
     * @see std::any
     * @note this function doesn't enforce type safety, so it's up to the derived classes to handle type checking
     */
    void setParameter(const std::string& key, const std::any& value);

    /**
    * @brief Get a parameter from the filter
    * 
    * @param key Parameter name
    * @return std::any The parameter value
    * @throws std::out_of_range If the parameter does not exist
    */
    std::any getParameter(const std::string& key) const {
        auto it = parameters.find(key);
        if (it == parameters.end()) {
            throw std::out_of_range("Parameter not found: " + key);
        }
        return it->second;
    }

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