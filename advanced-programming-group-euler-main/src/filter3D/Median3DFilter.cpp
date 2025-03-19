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
 #include <vector>
 #include <algorithm>
 #include <tuple>
 
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
                 
                 // Find median for each channel
                 unsigned char r_median, g_median = 0, b_median = 0, a_median = 255;
                 
                 // Red channel (always present)
                 size_t mid = r_values.size() / 2;
                 std::nth_element(r_values.begin(), r_values.begin() + mid, r_values.end());
                 r_median = r_values[mid];
                 
                 // Green channel (if present)
                 if (channels > 1) {
                     mid = g_values.size() / 2;
                     std::nth_element(g_values.begin(), g_values.begin() + mid, g_values.end());
                     g_median = g_values[mid];
                 }
                 
                 // Blue channel (if present)
                 if (channels > 2) {
                     mid = b_values.size() / 2;
                     std::nth_element(b_values.begin(), b_values.begin() + mid, b_values.end());
                     b_median = b_values[mid];
                 }
                 
                 // Alpha channel (if present)
                 if (channels > 3) {
                     mid = a_values.size() / 2;
                     std::nth_element(a_values.begin(), a_values.begin() + mid, a_values.end());
                     a_median = a_values[mid];
                 }
                 
                 // Set output voxel to median values
                 result->setVoxel(x, y, z, Pixel(r_median, g_median, b_median, a_median));
             }
         }
     }
     
     return result;
 }