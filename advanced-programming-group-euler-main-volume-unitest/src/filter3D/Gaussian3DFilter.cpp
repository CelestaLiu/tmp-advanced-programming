/**
 * @file Gaussian3DFilter.cpp
 * @brief Implementation of the Gaussian3DFilter class
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

 #include "Gaussian3DFilter.h"
 #include "../Volume.h"
 #include <cmath>
 #include <stdexcept>
 #include <tuple>
 
 // Constructor with kernel size and sigma
 Gaussian3DFilter::Gaussian3DFilter(int kernelSize, float sigma)
     : VolumeFilter("Gaussian3D", kernelSize), sigma(sigma) {
     // Generate the kernel
     kernel = generateKernel();
 }
 
 // Get the standard deviation (sigma) of the Gaussian filter
 float Gaussian3DFilter::getSigma() const {
     return sigma;
 }
 
 // Set the standard deviation (sigma) of the Gaussian filter
 void Gaussian3DFilter::setSigma(float sigma) {
     if (sigma <= 0) {
         throw std::invalid_argument("Sigma must be positive");
     }
     
     this->sigma = sigma;
     // Regenerate the kernel
     kernel = generateKernel();
 }
 
 // Generate the 3D Gaussian kernel
 std::vector<float> Gaussian3DFilter::generateKernel() const {
     // Calculate radius (half kernel size)
     int radius = kernelSize / 2;
     
     // Create flattened 3D kernel
     std::vector<float> k(kernelSize * kernelSize * kernelSize);
     
     // Calculate normalization constant
     float sigma2 = sigma * sigma;
     float norm = 1.0f / (std::pow(2.0f * M_PI * sigma2, 1.5f));
     
     // Calculate kernel values
     float sum = 0.0f; // For normalization
     
     for (int z = -radius; z <= radius; ++z) {
         for (int y = -radius; y <= radius; ++y) {
             for (int x = -radius; x <= radius; ++x) {
                 // Calculate 3D Gaussian function
                 float exp_val = -(x*x + y*y + z*z) / (2.0f * sigma2);
                 float value = norm * std::exp(exp_val);
                 
                 // Store value in flattened array
                 int idx = (z + radius) * kernelSize * kernelSize + 
                          (y + radius) * kernelSize + 
                          (x + radius);
                 k[idx] = value;
                 
                 // Accumulate for normalization
                 sum += value;
             }
         }
     }
     
     // Normalize to ensure the kernel sums to 1
     if (sum > 0) {
         for (float& value : k) {
             value /= sum;
         }
     }
     
     return k;
 }
 
 // Apply the Gaussian blur filter to a volume
 std::unique_ptr<Volume> Gaussian3DFilter::apply(const Volume& volume) const {
     // Get volume dimensions
     int width, height, depth;
     std::tie(width, height, depth) = volume.getDimensions3D();
     int channels = volume.getChannels();
     
     // Create output volume
     auto result = std::make_unique<Volume>(width, height, depth, channels, volume.getName() + "_gaussian");
     
     // Calculate radius (half kernel size)
     int radius = kernelSize / 2;
     
     // Apply convolution
     for (int z = 0; z < depth; ++z) {
         for (int y = 0; y < height; ++y) {
             for (int x = 0; x < width; ++x) {
                 float r_sum = 0.0f, g_sum = 0.0f, b_sum = 0.0f, a_sum = 0.0f;
                 float weight_sum = 0.0f;
                 
                 // Apply kernel
                 for (int kz = -radius; kz <= radius; ++kz) {
                     for (int ky = -radius; ky <= radius; ++ky) {
                         for (int kx = -radius; kx <= radius; ++kx) {
                             // Calculate source coordinates
                             int sz = z + kz;
                             int sy = y + ky;
                             int sx = x + kx;
                             
                             // Check if coordinates are valid
                             if (sz >= 0 && sz < depth && sy >= 0 && sy < height && sx >= 0 && sx < width) {
                                 // Get kernel value
                                 int k_idx = (kz + radius) * kernelSize * kernelSize + 
                                            (ky + radius) * kernelSize + 
                                            (kx + radius);
                                 float k_value = kernel[k_idx];
                                 
                                 // Get pixel value
                                 Pixel pixel = volume.getVoxel(sx, sy, sz);
                                 
                                 // Accumulate weighted values
                                 r_sum += pixel.getR() * k_value;
                                 g_sum += pixel.getG() * k_value;
                                 b_sum += pixel.getB() * k_value;
                                 a_sum += pixel.getA() * k_value;
                                 weight_sum += k_value;
                             }
                         }
                     }
                 }
                 
                 // Normalize if needed
                 if (weight_sum > 0) {
                     r_sum /= weight_sum;
                     g_sum /= weight_sum;
                     b_sum /= weight_sum;
                     a_sum /= weight_sum;
                 }
                 
                 // Set output voxel
                 unsigned char r = static_cast<unsigned char>(std::round(std::min(std::max(r_sum, 0.0f), 255.0f)));
                 unsigned char g = static_cast<unsigned char>(std::round(std::min(std::max(g_sum, 0.0f), 255.0f)));
                 unsigned char b = static_cast<unsigned char>(std::round(std::min(std::max(b_sum, 0.0f), 255.0f)));
                 unsigned char a = static_cast<unsigned char>(std::round(std::min(std::max(a_sum, 0.0f), 255.0f)));
                 
                 result->setVoxel(x, y, z, Pixel(r, g, b, a));
             }
         }
     }
     
     return result;
 }