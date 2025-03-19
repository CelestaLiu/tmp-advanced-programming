/**
 * @file MaxIntensityProj.cpp
 * @brief Implementation of the MaxIntensityProj class
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

#include "MaxIntensityProj.h"
#include "../Volume.h"
#include "../Image.h"
#include <algorithm>
#include <tuple>

// Constructor
MaxIntensityProj::MaxIntensityProj(int slabStart, int slabEnd, float threshold)
    : Projection(ProjectionType::MAXIMUM_INTENSITY, slabStart, slabEnd), threshold(threshold) {
}

// Set the intensity threshold
void MaxIntensityProj::setThreshold(float threshold) {
    this->threshold = std::max(0.0f, std::min(255.0f, threshold));
}

// Get the intensity threshold
float MaxIntensityProj::getThreshold() const {
    return threshold;
}

// Apply the maximum intensity projection to a volume
Image MaxIntensityProj::apply(const Volume& volume) const {
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
    
    // For each pixel position in the output image
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // Find maximum intensity along Z-axis
            float maxIntensity = -1.0f;
            Pixel maxPixel;
            
            for (int z = startZ; z <= endZ; ++z) {
                Pixel current = volume.getVoxel(x, y, z);
                float intensity = current.getLuminance();
                
                // Apply threshold if set
                if (intensity >= threshold && intensity > maxIntensity) {
                    maxIntensity = intensity;
                    maxPixel = current;
                }
            }
            
            // If no pixel met the threshold, use a black pixel
            if (maxIntensity < 0) {
                maxPixel = Pixel(0, 0, 0);
            }
            
            // Set the pixel with maximum intensity in the projection
            projection.setPixel(x, y, maxPixel);
        }
    }
    
    return projection;
}