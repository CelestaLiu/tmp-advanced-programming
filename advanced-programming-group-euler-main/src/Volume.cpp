/**
 * @file Volume.cpp
 * @brief Implementation of the Volume class
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

 #include "Volume.h"
 #include "Image.h"
 #include "./filter3D/VolumeFilter.h"
 #include "./filter3D/Gaussian3DFilter.h"
 #include "./filter3D/Median3DFilter.h"
 #include "./projectionFunc/Projection.h"
 #include "Slice.h"
 #include <stdexcept>
 #include <algorithm>
 #include <cmath>
 #include <sstream>
 #include <iomanip>
 #include <map>
 #include <filesystem>
 #include <regex>
 #include <iostream>  // Added this include for std::cerr and std::endl
 
 // Include STB image libraries
 #include "stb_image.h"
 #include "stb_image_write.h"
 
 // Constructor with dimensions
 Volume::Volume(int width, int height, int depth, int channels, const std::string& name)
     : DataContainer(width, height, name), depth(depth), channels(channels) {
     // Validate parameters
     if (depth <= 0) {
         throw std::invalid_argument("Volume depth must be positive");
     }
     if (channels <= 0 || channels > 4) {
         throw std::invalid_argument("Channels must be between 1 and 4");
     }
     
     // Initialize 3D voxel array with default (black) pixels
     voxels.resize(depth);
     for (int z = 0; z < depth; ++z) {
         voxels[z].resize(height);
         for (int y = 0; y < height; ++y) {
             voxels[z][y].resize(width, Pixel());
         }
     }
 }
 
 // Constructor from a series of image files
 Volume::Volume(const std::vector<std::string>& filenames, const std::string& name)
     : DataContainer(1, 1, name), depth(0), channels(0) {
     // Load from files, throw exception if loading fails
     if (!loadFromFiles(filenames)) {
         throw std::runtime_error("Failed to load volume from files: " + 
                                  (filenames.empty() ? "No files provided" : filenames[0] + " (and others)"));
     }
 }
 
 // Constructor from a base path with range of indices
 Volume::Volume(const std::string& basePath, int firstIndex, int lastIndex, 
                const std::string& extension, const std::string& name)
     : DataContainer(1, 1, name), depth(0), channels(0) {
     // Load from index range, throw exception if loading fails
     if (!loadFromIndexRange(basePath, firstIndex, lastIndex, extension)) {
         throw std::runtime_error("Failed to load volume from index range");
     }
 }
 
 // Constructor from a directory path
 Volume::Volume(const std::string& directoryPath, const std::string& extension, 
                const std::string& name)
     : DataContainer(1, 1, name), depth(0), channels(0) {
     // Load from directory, throw exception if loading fails
     if (!loadFromDirectory(directoryPath, extension)) {
         throw std::runtime_error("Failed to load volume from directory: " + directoryPath);
     }
 }
 
 // Destructor
 Volume::~Volume() {
     // No specific cleanup needed as std::vector handles memory automatically
 }
 
 // Get the depth of the volume
 int Volume::getDepth() const {
     return depth;
 }
 
 // Get the 3D dimensions of the volume
 std::tuple<int, int, int> Volume::getDimensions3D() const {
     return std::make_tuple(width, height, depth);
 }
 
 // Check if 3D coordinates are within bounds
 bool Volume::isInBounds3D(int x, int y, int z) const {
     return (x >= 0 && x < width && y >= 0 && y < height && z >= 0 && z < depth);
 }
 
 // Load volume data from a series of image files
 bool Volume::loadFromFiles(const std::vector<std::string>& filenames) {
     if (filenames.empty()) {
         return false;
     }
     
     // Store filenames for reference
     sliceFilenames = filenames;
     
     // Load first image to get dimensions
     int w, h, c;
     unsigned char* data = stbi_load(filenames[0].c_str(), &w, &h, &c, 0);
     if (!data) {
         return false;
     }
     
     // Update dimensions
     width = w;
     height = h;
     depth = filenames.size();
     channels = c;
     
     // Initialize voxels array with the correct dimensions
     voxels.resize(depth);
     for (int z = 0; z < depth; ++z) {
         voxels[z].resize(height);
         for (int y = 0; y < height; ++y) {
             voxels[z][y].resize(width);
         }
     }
     
     // Free the first image data as we'll reload it with the other slices
     stbi_image_free(data);
     
     // Load all slices sequentially
     for (int z = 0; z < depth; ++z) {
         int sliceChannels;
         data = stbi_load(filenames[z].c_str(), &w, &h, &sliceChannels, 0);
         if (!data) {
             return false;
         }
         
         // Check if all slices have the same dimensions
         if (w != width || h != height) {
             stbi_image_free(data);
             throw std::runtime_error("All slices must have the same dimensions. " + 
                                     filenames[z] + " differs from " + filenames[0]);
         }
         
         // Copy data to voxels
         for (int y = 0; y < height; ++y) {
             for (int x = 0; x < width; ++x) {
                 int index = (y * width + x) * sliceChannels;
                 unsigned char r = data[index];
                 unsigned char g = (sliceChannels > 1) ? data[index + 1] : r;
                 unsigned char b = (sliceChannels > 2) ? data[index + 2] : r;
                 unsigned char a = (sliceChannels > 3) ? data[index + 3] : 255;
                 voxels[z][y][x] = Pixel(r, g, b, a);
             }
         }
         
         // Free image data after copying
         stbi_image_free(data);
     }
     
     return true;
 }
 
 // Load volume data from a base path with range of indices
 bool Volume::loadFromIndexRange(const std::string& basePath, int firstIndex, int lastIndex, 
                                const std::string& extension) {
     if (firstIndex > lastIndex) {
         throw std::invalid_argument("First index must be less than or equal to last index");
     }
     
     // Create filenames from index range
     std::vector<std::string> filenames;
     for (int i = firstIndex; i <= lastIndex; ++i) {
         std::filesystem::path fullPath;
         
         // Check if basePath is a directory or a filename pattern
         if (std::filesystem::is_directory(basePath)) {
             // If it's a directory, append index and extension
             fullPath = std::filesystem::path(basePath) / 
                       (std::to_string(i) + "." + extension);
         } else {
             // If it's a pattern, construct the filename
             // First, check if basePath already has extension
             std::string baseWithoutExt = basePath;
             size_t dotPos = basePath.rfind('.');
             if (dotPos != std::string::npos) {
                 baseWithoutExt = basePath.substr(0, dotPos);
             }
             
             fullPath = baseWithoutExt + std::to_string(i) + "." + extension;
         }
         
         // Check if file exists
         if (!std::filesystem::exists(fullPath)) {
             std::cerr << "Warning: File not found: " << fullPath.string() << std::endl;
             continue;
         }
         
         filenames.push_back(fullPath.string());
     }
     
     if (filenames.empty()) {
         std::cerr << "Error: No valid files found in the specified index range." << std::endl;
         return false;
     }
     
     // Load volume from collected filenames
     return loadFromFiles(filenames);
 }
 
 // Load volume data from a directory path
 bool Volume::loadFromDirectory(const std::string& directoryPath, const std::string& extension) {
     if (!std::filesystem::is_directory(directoryPath)) {
         throw std::invalid_argument("Not a valid directory: " + directoryPath);
     }
     
     // Collect all files with the specified extension
     std::vector<std::string> filenames;
     std::string dotExtension = "." + extension;
     
     for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
         if (entry.is_regular_file() && entry.path().extension() == dotExtension) {
             filenames.push_back(entry.path().string());
         }
     }
     
     // Sort filenames to ensure correct order
     std::sort(filenames.begin(), filenames.end());
     
     if (filenames.empty()) {
         std::cerr << "Error: No files with extension ." << extension 
                   << " found in directory: " << directoryPath << std::endl;
         return false;
     }
     
     // Load volume from collected filenames
     return loadFromFiles(filenames);
 }
 
 // Get a pixel (voxel) at a specific position
 Pixel Volume::getVoxel(int x, int y, int z) const {
     if (!isInBounds3D(x, y, z)) {
         throw std::out_of_range("Voxel coordinates out of bounds: (" + 
                                std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) + ")");
     }
     return voxels[z][y][x];
 }
 
 // Set a pixel (voxel) at a specific position
 void Volume::setVoxel(int x, int y, int z, const Pixel& pixel) {
     if (!isInBounds3D(x, y, z)) {
         throw std::out_of_range("Voxel coordinates out of bounds: (" + 
                                std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) + ")");
     }
     voxels[z][y][x] = pixel;
 }
 
 // Save all slices of the volume to files
 bool Volume::saveToFiles(const std::string& baseFilename, const std::string& extension) const {
     bool allSucceeded = true;
     
     // Save each slice with a sequentially numbered filename
     for (int z = 0; z < depth; ++z) {
         // Format slice number with leading zeros (e.g., slice_0001.png)
         std::ostringstream sliceStr;
         sliceStr << baseFilename << "_" << std::setw(4) << std::setfill('0') << z << extension;
         std::string filename = sliceStr.str();
         
         // Save this slice and track success
         if (!saveSliceToFile(filename, z)) {
             allSucceeded = false;
         }
     }
     
     return allSucceeded;
 }
 
 // Save a specific slice of the volume to a file
 bool Volume::saveSliceToFile(const std::string& filename, int sliceIndex) const {
     if (sliceIndex < 0 || sliceIndex >= depth) {
         throw std::out_of_range("Slice index out of bounds: " + std::to_string(sliceIndex));
     }
     
     // Create data array for stb_image_write
     unsigned char* data = new unsigned char[width * height * channels];
     
     // Copy slice data to array
     for (int y = 0; y < height; ++y) {
         for (int x = 0; x < width; ++x) {
             Pixel p = voxels[sliceIndex][y][x];
             int index = (y * width + x) * channels;
             data[index] = p.getR();
             if (channels > 1) data[index + 1] = p.getG();
             if (channels > 2) data[index + 2] = p.getB();
             if (channels > 3) data[index + 3] = p.getA();
         }
     }
     
     // Write to file
     int success = stbi_write_png(filename.c_str(), width, height, channels, data, 0);
     
     // Clean up
     delete[] data;
     
     return success != 0;
 }
 
 // Apply a filter to the volume
 std::unique_ptr<Volume> Volume::applyFilter(VolumeFilter& filter) const {
     // Delegate the filtering operation to the VolumeFilter object
     return filter.apply(*this);
 }
 
 // Apply a Gaussian 3D filter to the volume
 std::unique_ptr<Volume> Volume::applyGaussianFilter(int kernelSize, float sigma) const {
     // Validate kernel size
     if (kernelSize % 2 == 0) {
         throw std::invalid_argument("Kernel size must be odd");
     }
     
     // Create a Gaussian3DFilter and apply it
     Gaussian3DFilter filter(kernelSize, sigma);
     return filter.apply(*this);
 }
 
 // Apply a Median 3D filter to the volume
 std::unique_ptr<Volume> Volume::applyMedianFilter(int kernelSize) const {
     // Validate kernel size
     if (kernelSize % 2 == 0) {
         throw std::invalid_argument("Kernel size must be odd");
     }
     
     // Create a Median3DFilter and apply it
     Median3DFilter filter(kernelSize);
     return filter.apply(*this);
 }
 
 // Apply a projection to generate a 2D image
 Image Volume::applyProjection(const Projection& projection) const {
     // Delegate the projection operation to the Projection object
     return projection.apply(*this);
 }
 
 // Extract a slice from the volume
 Image Volume::extractSlice(const Slice& slice) const {
     // Delegate the slice extraction to the Slice object
     return slice.extract(*this);
 }
 
 // DataContainer interface implementations
 
 // Get a pixel from the middle slice
 Pixel Volume::getPixel(int x, int y) const {
     if (!isInBounds(x, y)) {
         throw std::out_of_range("Pixel coordinates out of bounds: (" + 
                                std::to_string(x) + "," + std::to_string(y) + ")");
     }
     // Return the pixel from the middle slice (depth/2)
     return voxels[depth / 2][y][x];
 }
 
 // Set a pixel in the middle slice
 void Volume::setPixel(int x, int y, const Pixel& pixel) {
     if (!isInBounds(x, y)) {
         throw std::out_of_range("Pixel coordinates out of bounds: (" + 
                                std::to_string(x) + "," + std::to_string(y) + ")");
     }
     // Set the pixel in the middle slice (depth/2)
     voxels[depth / 2][y][x] = pixel;
 }
 
 // Create a deep copy of the volume
 std::unique_ptr<DataContainer> Volume::clone() const {
     // Create a new volume with the same dimensions and name
     auto copy = std::make_unique<Volume>(width, height, depth, channels, name);
     
     // Copy all voxels
     for (int z = 0; z < depth; ++z) {
         for (int y = 0; y < height; ++y) {
             for (int x = 0; x < width; ++x) {
                 copy->setVoxel(x, y, z, getVoxel(x, y, z));
             }
         }
     }
     
     return copy;
 }
 
 // Save the middle slice to a file (to satisfy DataContainer interface)
 bool Volume::saveToFile(const std::string& filename) const {
     // Use the middle slice (depth/2) as a representative 2D image
     return saveSliceToFile(filename, depth / 2);
 }
 
 // Get the number of channels in the volume
 int Volume::getChannels() const {
     return channels;
 }
 
 // Apply a filter to the volume (string-based interface from DataContainer)
 std::unique_ptr<DataContainer> Volume::applyFilter(const std::string& filterName, 
                                                   const std::string& parameters) const {
     // Parse parameters from the string
     // Format expected: "param1=value1,param2=value2,..."
     std::map<std::string, std::string> paramMap;
     
     if (!parameters.empty()) {
         std::stringstream ss(parameters);
         std::string param;
         
         while (std::getline(ss, param, ',')) {
             size_t pos = param.find('=');
             if (pos != std::string::npos) {
                 std::string key = param.substr(0, pos);
                 std::string value = param.substr(pos + 1);
                 paramMap[key] = value;
             }
         }
     }
     
     // Default parameters
     int kernelSize = 3;
     float sigma = 2.0f;
     
     // Extract parameters if they exist
     if (paramMap.count("kernelSize") > 0) {
         try {
             kernelSize = std::stoi(paramMap["kernelSize"]);
         } catch (...) {
             // Use default if parsing fails
         }
     }
     
     if (paramMap.count("sigma") > 0) {
         try {
             sigma = std::stof(paramMap["sigma"]);
         } catch (...) {
             // Use default if parsing fails
         }
     }
     
     // Apply the appropriate filter based on the name
     if (filterName == "gaussian3d") {
         return applyGaussianFilter(kernelSize, sigma);
     } 
     else if (filterName == "median3d") {
         return applyMedianFilter(kernelSize);
     }
     else {
         // Unknown filter name - return an unmodified copy
         return clone();
     }
 }