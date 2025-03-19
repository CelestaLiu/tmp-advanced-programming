/**
 * @file Projection.cpp
 * @brief Implementation of the abstract Projection class
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

#include "Projection.h"
#include "../Volume.h"
#include <stdexcept>

// Constructor with projection type
Projection::Projection(ProjectionType type, int slabStart, int slabEnd)
    : type(type), slabStart(slabStart), slabEnd(slabEnd) {
}

// Virtual destructor
Projection::~Projection() {
    // No specific cleanup needed
}

// Get the projection type
ProjectionType Projection::getType() const {
    return type;
}

// Set the slab range for the projection
void Projection::setSlabRange(int start, int end) {
    // Ensure start <= end, but allow end to be -1 (meaning the last slice)
    if (end != -1 && start > end) {
        throw std::invalid_argument("Slab start index must be less than or equal to end index");
    }

    // Ensure start is non-negative
    if (start < 0) {
        throw std::invalid_argument("Slab start index must be non-negative");
    }
    
    slabStart = start;
    slabEnd = end;
}