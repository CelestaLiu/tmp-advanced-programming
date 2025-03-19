/**
 * @file VolumeFilter.cpp
 * @brief Implementation of the abstract VolumeFilter class
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

#include "VolumeFilter.h"
#include <stdexcept>

// Constructor with filter name and kernel size
VolumeFilter::VolumeFilter(const std::string& name, int kernelSize)
    : name(name), kernelSize(kernelSize) {
    // Validate kernel size
    if (kernelSize % 2 == 0) {
        throw std::invalid_argument("Kernel size must be odd");
    }
    if (kernelSize < 1) {
        throw std::invalid_argument("Kernel size must be positive");
    }
}

// Virtual destructor
VolumeFilter::~VolumeFilter() {
    // No specific cleanup needed
}

// Get the filter name
std::string VolumeFilter::getName() const {
    return name;
}

// Get the kernel size
int VolumeFilter::getKernelSize() const {
    return kernelSize;
}

// Set the kernel size
void VolumeFilter::setKernelSize(int kernelSize) {
    // Validate kernel size
    if (kernelSize % 2 == 0) {
        throw std::invalid_argument("Kernel size must be odd");
    }
    if (kernelSize < 1) {
        throw std::invalid_argument("Kernel size must be positive");
    }
    
    this->kernelSize = kernelSize;
}