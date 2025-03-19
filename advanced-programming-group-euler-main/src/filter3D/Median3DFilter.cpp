/**
 * @file Median3DFilter.cpp
 * @brief Implementation of the Median3DFilter class
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

 #include "Median3DFilter.h"
 #include "../Volume.h"
 #include <iostream>
 #include <vector>
 #include <algorithm>
 #include <tuple>
 #include <array>
 
 // Anonymous namespace to make this function local to this file
 namespace {
     // Helper function to find median using counting approach
     // This avoids using nth_element which is not allowed
     unsigned char findMedian(const std::vector<unsigned char>& values) {
         // Since unsigned char has a range of 0-255, we can use a counting approach
         std::array<int, 256> counts = {0}; // Initialize all counts to 0
         
         // Count occurrences of each value
         for (unsigned char val : values) {
             counts[val]++;
         }
         
         // Find the middle position
         size_t totalElements = values.size();
         size_t medianPosition = totalElements / 2;
         
         // Walk through counts until we reach the median position
         size_t currentPos = 0;
         for (int i = 0; i < 256; ++i) {
             currentPos += counts[i];
             if (currentPos > medianPosition) {
                 return static_cast<unsigned char>(i);
             }
         }
         
         // In case of empty vector (shouldn't happen)
         return 0;
     }
 }
 
 // Constructor with kernel size
 Median3DFilter::Median3DFilter(int kernelSize)
     : VolumeFilter("Median3D", kernelSize) {
 }
 
 // Apply the median filter to a volume
 std::unique_ptr<Volume> Median3DFilter::apply(const Volume& volume) const {
     // Get volume dimensions
     int width, height, depth;
     std::tie(width, height, depth) = volume.getDimensions3D();
     int channels = volume.getChannels();
     
     // Create output volume
     auto result = std::make_unique<Volume>(width, height, depth, channels, volume.getName() + "_median");
     
     // Calculate radius (half kernel size)
     int radius = kernelSize / 2;
     
     // Create temporary vectors to store neighborhood values for each channel
     std::vector<unsigned char> r_values, g_values, b_values, a_values;
     int maxNeighborhood = kernelSize * kernelSize * kernelSize;
     r_values.reserve(maxNeighborhood);
     if (channels > 1) g_values.reserve(maxNeighborhood);
     if (channels > 2) b_values.reserve(maxNeighborhood);
     if (channels > 3) a_values.reserve(maxNeighborhood);
     
     // Apply median filter to each voxel
     for (int z = 0; z < depth; ++z) {
         for (int y = 0; y < height; ++y) {
             for (int x = 0; x < width; ++x) {
                 // Clear the vectors
                 r_values.clear();
                 g_values.clear();
                 b_values.clear();
                 a_values.clear();
                 
                 // Collect neighborhood values
                 for (int kz = -radius; kz <= radius; ++kz) {
                     for (int ky = -radius; ky <= radius; ++ky) {
                         for (int kx = -radius; kx <= radius; ++kx) {
                             // Calculate source coordinates
                             int sz = z + kz;
                             int sy = y + ky;
                             int sx = x + kx;
                             
                             // Check if coordinates are valid
                             if (sz >= 0 && sz < depth && sy >= 0 && sy < height && sx >= 0 && sx < width) {
                                 // Get pixel value
                                 Pixel pixel = volume.getVoxel(sx, sy, sz);
                                 
                                 // Store channel values
                                 r_values.push_back(pixel.getR());
                                 if (channels > 1) g_values.push_back(pixel.getG());
                                 if (channels > 2) b_values.push_back(pixel.getB());
                                 if (channels > 3) a_values.push_back(pixel.getA());
                             }
                         }
                     }
                 }
                 
                 // Find median for each channel using our custom median finder
                 unsigned char r_median, g_median = 0, b_median = 0, a_median = 255;
                 
                 // Red channel (always present)
                 r_median = findMedian(r_values);
                 
                 // Green channel (if present)
                 if (channels > 1) {
                     g_median = findMedian(g_values);
                 }
                 
                 // Blue channel (if present)
                 if (channels > 2) {
                     b_median = findMedian(b_values);
                 }
                 
                 // Alpha channel (if present)
                 if (channels > 3) {
                     a_median = findMedian(a_values);
                 }
                 
                 // Set output voxel to median values
                 result->setVoxel(x, y, z, Pixel(r_median, g_median, b_median, a_median));
             }
         }
     }
     
     return result;
 }