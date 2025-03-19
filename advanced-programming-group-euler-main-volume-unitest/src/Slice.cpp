/**
 * @file Slice.cpp
 * @brief Implementation of the Slice class
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

 #include "Slice.h"
 #include "Volume.h"
 #include "Image.h"
 #include <stdexcept>
 #include <tuple>
 #include <algorithm>
 #include <iostream>
 
 // Default constructor
 Slice::Slice() : plane(SlicePlane::XY), position(0) {
 }
 
 // Constructor with slice plane and position
 Slice::Slice(SlicePlane plane, int position) : plane(plane), position(position - 1) {
     // Convert 1-based position index (from user input) to 0-based index
     // The position is now 0-based inside the class
     std::cout << "Debug: Created slice with plane " << static_cast<int>(plane) 
               << " and position " << position << " (internal position: " << this->position << ")" << std::endl;
 }
 
 // Getters
 SlicePlane Slice::getPlane() const {
     return plane;
 }
 
 int Slice::getPosition() const {
     return position;
 }
 
 // Setters
 void Slice::setPlane(SlicePlane plane) {
     this->plane = plane;
 }
 
 void Slice::setPosition(int position) {
     this->position = position;
 }
 
 // Extract a 2D slice from the given volume
 Image Slice::extract(const Volume& volume) const {
     // Get volume dimensions
     int width, height, depth;
     std::tie(width, height, depth) = volume.getDimensions3D();
     int channels = volume.getChannels();
     
     std::cout << "Debug: Extracting slice from volume with dimensions " 
               << width << "x" << height << "x" << depth 
               << " at position " << position << " for plane " << static_cast<int>(plane) << std::endl;
     
     // Validate position based on the plane
     switch (plane) {
         case SlicePlane::XY:
             if (position < 0 || position >= depth) {
                 throw std::out_of_range("Z-position is out of range: " + std::to_string(position) + 
                                         " (valid range: 0-" + std::to_string(depth - 1) + ")");
             }
             break;
         case SlicePlane::XZ:
             if (position < 0 || position >= height) {
                 throw std::out_of_range("Y-position is out of range: " + std::to_string(position) + 
                                         " (valid range: 0-" + std::to_string(height - 1) + ")");
             }
             break;
         case SlicePlane::YZ:
             if (position < 0 || position >= width) {
                 throw std::out_of_range("X-position is out of range: " + std::to_string(position) + 
                                         " (valid range: 0-" + std::to_string(width - 1) + ")");
             }
             break;
     }
     
     // Create the output image with correct dimensions
     int sliceWidth = 0, sliceHeight = 0;
     
     switch (plane) {
         case SlicePlane::XY:
             sliceWidth = width;
             sliceHeight = height;
             break;
         case SlicePlane::XZ:
             sliceWidth = width;
             sliceHeight = depth;
             break;
         case SlicePlane::YZ:
             sliceWidth = height;
             sliceHeight = depth;
             break;
     }
     
     std::cout << "Debug: Creating slice image with dimensions " 
               << sliceWidth << "x" << sliceHeight << "x" << channels << std::endl;
     
     // Make sure dimensions are positive
     if (sliceWidth <= 0 || sliceHeight <= 0) {
         throw std::invalid_argument("Slice dimensions must be positive: " + 
                                    std::to_string(sliceWidth) + "x" + std::to_string(sliceHeight));
     }
     
     // Create the slice image
     Image slice(sliceWidth, sliceHeight, channels);
     
     // Extract the appropriate slice
     switch (plane) {
         case SlicePlane::XY:
             // XY plane (constant Z)
             for (int y = 0; y < height; ++y) {
                 for (int x = 0; x < width; ++x) {
                     slice.setPixel(x, y, volume.getVoxel(x, y, position));
                 }
             }
             break;
             
         case SlicePlane::XZ:
             // XZ plane (constant Y)
             for (int z = 0; z < depth; ++z) {
                 for (int x = 0; x < width; ++x) {
                     slice.setPixel(x, z, volume.getVoxel(x, position, z));
                 }
             }
             break;
             
         case SlicePlane::YZ:
             // YZ plane (constant X)
             for (int z = 0; z < depth; ++z) {
                 for (int y = 0; y < height; ++y) {
                     slice.setPixel(y, z, volume.getVoxel(position, y, z));
                 }
             }
             break;
     }
     
     return slice;
 }