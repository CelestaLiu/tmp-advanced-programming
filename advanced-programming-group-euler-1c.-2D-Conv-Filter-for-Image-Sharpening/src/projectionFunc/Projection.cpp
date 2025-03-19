/**
 * @file Projection.cpp
 * @brief Implementation of the abstract Projection class
 * @group [GROUP_NAME]
 * 
 * Group members:
 * - [MEMBER1] ([USERNAME1])
 * - [MEMBER2] ([USERNAME2])
 * - [MEMBER3] ([USERNAME3])
 * - [MEMBER4] ([USERNAME4])
 * - [MEMBER5] ([USERNAME5])
 * - [MEMBER6] ([USERNAME6])
 * - [MEMBER7] ([USERNAME7])
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
    
    slabStart = start;
    slabEnd = end;
}