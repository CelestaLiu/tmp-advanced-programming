/**
 * @file AvgIntensityProj.cpp
 * @brief Implementation of the AvgIntensityProj class
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

#include "AvgIntensityProj.h"
#include "../Volume.h"
#include "../Image.h"
#include <algorithm>
#include <tuple>
#include <vector>
#include <cmath>

// Constructor
AvgIntensityProj::AvgIntensityProj(int slabStart, int slabEnd, bool useMedian)
    : Projection(ProjectionType::AVERAGE_INTENSITY, slabStart, slabEnd), useMedian(useMedian) {
}

// Set whether to use median instead of mean
void AvgIntensityProj::setUseMedian(bool useMedian) {
    this->useMedian = useMedian;
}

// Check if using median instead of mean
bool AvgIntensityProj::isUsingMedian() const {
    return useMedian;
}

// Apply the average intensity projection to a volume
Image AvgIntensityProj::apply(const Volume& volume) const {
    // Get volume dimensions
    int width, height, depth;
    std::tie(width, height, depth) = volume.getDimensions3D();
    
    // Validate slab range
    int startZ = slabStart;
    int endZ = (slabEnd == -1) ? depth - 1 : slabEnd;
    
    // Ensure bounds are valid
    startZ = std::max(0, std::min(startZ, depth - 1));
    endZ = std::max(startZ, std::min(endZ, depth - 1));
    
    // Number of slices in the slab
    int slabDepth = endZ - startZ + 1;
    
    // Create output image with the same number of channels as the volume
    Image projection(width, height, volume.getChannels());
    
    if (useMedian) {
        // For each pixel position in the output image
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                // Collect all values for this (x,y) position along Z
                std::vector<unsigned char> valuesR, valuesG, valuesB, valuesA;
                
                for (int z = startZ; z <= endZ; ++z) {
                    Pixel current = volume.getVoxel(x, y, z);
                    valuesR.push_back(current.getR());
                    valuesG.push_back(current.getG());
                    valuesB.push_back(current.getB());
                    valuesA.push_back(current.getA());
                }
                
                // Calculate median by sorting and taking the middle value
                size_t mid = valuesR.size() / 2;
                
                std::nth_element(valuesR.begin(), valuesR.begin() + mid, valuesR.end());
                std::nth_element(valuesG.begin(), valuesG.begin() + mid, valuesG.end());
                std::nth_element(valuesB.begin(), valuesB.begin() + mid, valuesB.end());
                std::nth_element(valuesA.begin(), valuesA.begin() + mid, valuesA.end());
                
                unsigned char medianR = valuesR[mid];
                unsigned char medianG = valuesG[mid];
                unsigned char medianB = valuesB[mid];
                unsigned char medianA = valuesA[mid];
                
                // Set the median pixel in the projection
                projection.setPixel(x, y, Pixel(medianR, medianG, medianB, medianA));
            }
        }
    } else {
        // For each pixel position in the output image (using mean)
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                // Accumulate RGB values along Z-axis
                unsigned long sumR = 0, sumG = 0, sumB = 0, sumA = 0;
                
                for (int z = startZ; z <= endZ; ++z) {
                    Pixel current = volume.getVoxel(x, y, z);
                    sumR += current.getR();
                    sumG += current.getG();
                    sumB += current.getB();
                    sumA += current.getA();
                }
                
                // Calculate average RGB values
                unsigned char avgR = static_cast<unsigned char>(sumR / slabDepth);
                unsigned char avgG = static_cast<unsigned char>(sumG / slabDepth);
                unsigned char avgB = static_cast<unsigned char>(sumB / slabDepth);
                unsigned char avgA = static_cast<unsigned char>(sumA / slabDepth);
                
                // Set the average pixel in the projection
                projection.setPixel(x, y, Pixel(avgR, avgG, avgB, avgA));
            }
        }
    }
    
    return projection;
}