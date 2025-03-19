/**
 * @file MinIntensityProj.cpp
 * @brief Implementation of the MinIntensityProj class
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

#include "MinIntensityProj.h"
#include "../Volume.h"
#include "../Image.h"
#include <algorithm>
#include <tuple>

// Constructor
MinIntensityProj::MinIntensityProj(int slabStart, int slabEnd)
    : Projection(ProjectionType::MINIMUM_INTENSITY, slabStart, slabEnd) {
}

// Apply the minimum intensity projection to a volume
Image MinIntensityProj::apply(const Volume& volume) const {
    // Get volume dimensions
    int width, height, depth;
    std::tie(width, height, depth) = volume.getDimensions3D();
    
    // Validate slab range
    int startZ = slabStart;
    int endZ = (slabEnd == -1) ? depth - 1 : slabEnd;
    
    // Ensure bounds are valid
    startZ = std::max(0, std::min(startZ, depth - 1));
    endZ = std::max(startZ, std::min(endZ, depth - 1));
    
    // Create output image with the same number of channels as the volume
    Image projection(width, height, volume.getChannels());
    
    // Initialize with maximum possible intensity
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            projection.setPixel(x, y, Pixel(255, 255, 255));
        }
    }
    
    // For each pixel position in the output image
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // Find minimum intensity along Z-axis
            float minIntensity = 256.0f; // Just above maximum possible intensity
            Pixel minPixel;
            
            for (int z = startZ; z <= endZ; ++z) {
                Pixel current = volume.getVoxel(x, y, z);
                float intensity = current.getLuminance();
                
                if (intensity < minIntensity) {
                    minIntensity = intensity;
                    minPixel = current;
                }
            }
            
            // Set the pixel with minimum intensity in the projection
            projection.setPixel(x, y, minPixel);
        }
    }
    
    return projection;
}