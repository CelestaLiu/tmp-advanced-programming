/**
 * @file AvgIntensityProj.cpp
 * @brief Implementation of the AvgIntensityProj class
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

 #include "AvgIntensityProj.h"
 #include "../Volume.h"
 #include "../Image.h"
 #include <algorithm>
 #include <tuple>
 #include <vector>
 #include <cmath>
 #include <array>
 
 // Anonymous namespace to make this function local to this file
 namespace {
     // Helper function to find median using counting approach
     // This avoids using nth_element which is not allowed
     unsigned char findMedianValue(const std::vector<unsigned char>& values) {
        // Since unsigned char has a range of 0-255, we can use a counting approach
        std::array<int, 256> counts = {0}; // Initialize all counts to 0
        
        // Count occurrences of each value
        for (unsigned char val : values) {
            counts[val]++;
        }
        
        // Find the middle position(s)
        size_t totalElements = values.size();
        bool isEven = (totalElements % 2 == 0);
        size_t medianPosition1 = totalElements / 2 - (isEven ? 1 : 0);
        size_t medianPosition2 = totalElements / 2;
        
        // Find the first median value
        size_t currentPos = 0;
        int firstMedianValue = 0;
        for (int i = 0; i < 256; ++i) {
            currentPos += counts[i];
            if (currentPos > medianPosition1) {
                firstMedianValue = i;
                break;
            }
        }
        
        // If odd-sized array, return the first median value
        if (!isEven) {
            return static_cast<unsigned char>(firstMedianValue);
        }
        
        // For even-sized arrays, find the second median value
        int secondMedianValue = firstMedianValue; // Default if no higher value exists
        if (currentPos > medianPosition2) {
            // The second value is the same as the first
        } else {
            for (int i = firstMedianValue + 1; i < 256; ++i) {
                currentPos += counts[i];
                if (currentPos > medianPosition2) {
                    secondMedianValue = i;
                    break;
                }
            }
        }
        
        // Return the average (rounded)
        return static_cast<unsigned char>((firstMedianValue + secondMedianValue + 1) / 2);
        // Note: The + 1 implements rounding instead of truncation
    }
 }
 
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
                 
                 // Calculate median using our custom median finder instead of nth_element
                 unsigned char medianR = findMedianValue(valuesR);
                 unsigned char medianG = findMedianValue(valuesG);
                 unsigned char medianB = findMedianValue(valuesB);
                 unsigned char medianA = findMedianValue(valuesA);
                 
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