/**
 * @file Filter.cpp
 * @brief Implementation of the abstract Filter class
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

 #include "Filter.h"

 // *******************************************************************************************
 // -------------------------------------------------------------------------------------------
 // CONSTRUCTORS & DESTRUCTOR
 // -------------------------------------------------------------------------------------------
 // *******************************************************************************************
 
//Constructor for the Filter class
 Filter::Filter(const std::string& filterName)
     : name(filterName) {}

// Virtual destructor for Filter
 Filter::~Filter() = default;
 
 // *******************************************************************************************
 // -------------------------------------------------------------------------------------------
 // GETTERS & SETTERS
 // -------------------------------------------------------------------------------------------
 // *******************************************************************************************
 
// Get Name of the filter
 std::string Filter::getName() const {
     return name;
 }

// Set Parameter for the filter
void Filter::setParameter(const std::string& key, const std::any& value) {
    if (value.type() == typeid(int)) {
        parameters[key] = std::any_cast<int>(value);
    } else if (value.type() == typeid(double)) {
        parameters[key] = std::any_cast<double>(value);
    } else if (value.type() == typeid(std::string)) {
        parameters[key] = std::any_cast<std::string>(value);
    } else {
        throw std::invalid_argument("Unsupported parameter type.");
    }
}
