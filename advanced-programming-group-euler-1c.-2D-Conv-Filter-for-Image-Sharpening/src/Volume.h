/**
 * @file Volume.h
 * @brief Definition of the Volume class for 3D data processing
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

 #ifndef VOLUME_H
 #define VOLUME_H
 
 #include "DataContainer.h"
 #include <vector>
 #include <string>
 #include <memory>
 #include <tuple>
 #include <filesystem>
 
 // Forward declarations
 class VolumeFilter;
 class Gaussian3DFilter;
 class Median3DFilter;
 class Projection;
 class Slice;
 class Image;
 
 /**
  * @brief Class representing a 3D volume of voxels
  * 
  * The Volume class extends DataContainer to handle 3D data, providing storage
  * and access methods for voxels. Operations like projections, slicing, and
  * filtering are delegated to specialized classes.
  */
 class Volume : public DataContainer {
 private:
     std::vector<std::vector<std::vector<Pixel>>> voxels; ///< 3D array of voxels [z][y][x]
     int depth; ///< Depth (z-dimension) of the volume
     std::vector<std::string> sliceFilenames; ///< Filenames of the slices that make up the volume
     int channels; ///< Number of color channels in the volume
 
 public:
     /**
      * @brief Constructor with dimensions
      * 
      * @param width Width (x-dimension) of the volume
      * @param height Height (y-dimension) of the volume
      * @param depth Depth (z-dimension) of the volume
      * @param channels Number of color channels (default: 3 for RGB)
      * @param name Optional name/identifier for the volume
      * @throws std::invalid_argument If dimensions are not positive
      */
     Volume(int width, int height, int depth, int channels = 3, const std::string& name = "");
 
     /**
      * @brief Constructor from a series of image files
      * 
      * @param filenames Vector of filenames to load slices from
      * @param name Optional name/identifier for the volume
      * @throws std::runtime_error If unable to load from files
      */
     Volume(const std::vector<std::string>& filenames, const std::string& name = "");
     
     /**
      * @brief Constructor from a base path with range of indices
      * 
      * @param basePath Base path for loading slice images
      * @param firstIndex First index to load (inclusive)
      * @param lastIndex Last index to load (inclusive)
      * @param extension File extension (without dot, e.g., "png")
      * @param name Optional name/identifier for the volume
      * @throws std::runtime_error If unable to load from files
      */
     Volume(const std::string& basePath, int firstIndex, int lastIndex, 
            const std::string& extension = "png", const std::string& name = "");
 
     /**
      * @brief Constructor from a directory path
      * 
      * @param directoryPath Directory containing volume slices
      * @param extension File extension to filter by (without dot, e.g., "png")
      * @param name Optional name/identifier for the volume
      * @throws std::runtime_error If unable to load from directory
      */
     Volume(const std::string& directoryPath, const std::string& extension = "png", 
            const std::string& name = "");
 
     /**
      * @brief Destructor
      */
     virtual ~Volume();
 
     /**
      * @brief Get the depth of the volume
      * 
      * @return int The depth (z-dimension)
      */
     int getDepth() const;
 
     /**
      * @brief Get the 3D dimensions of the volume
      * 
      * @return std::tuple<int, int, int> A tuple containing (width, height, depth)
      */
     std::tuple<int, int, int> getDimensions3D() const;
 
     /**
      * @brief Check if 3D coordinates are within bounds
      * 
      * @param x X-coordinate
      * @param y Y-coordinate
      * @param z Z-coordinate
      * @return true If (x,y,z) is within the volume bounds
      * @return false Otherwise
      */
     bool isInBounds3D(int x, int y, int z) const;
 
     /**
      * @brief Load volume data from a series of image files
      * 
      * @param filenames Vector of filenames to load slices from
      * @return bool True if loading was successful, false otherwise
      * @throws std::runtime_error If slices have inconsistent dimensions
      */
     bool loadFromFiles(const std::vector<std::string>& filenames);
     
     /**
      * @brief Load volume data from a base path with range of indices
      * 
      * @param basePath Base path for loading slice images
      * @param firstIndex First index to load (inclusive)
      * @param lastIndex Last index to load (inclusive)
      * @param extension File extension (without dot, e.g., "png")
      * @return bool True if loading was successful, false otherwise
      * @throws std::runtime_error If slices have inconsistent dimensions
      */
     bool loadFromIndexRange(const std::string& basePath, int firstIndex, int lastIndex, 
                            const std::string& extension = "png");
                            
     /**
      * @brief Load volume data from a directory path
      * 
      * @param directoryPath Directory containing volume slices
      * @param extension File extension to filter by (without dot, e.g., "png")
      * @return bool True if loading was successful, false otherwise
      * @throws std::runtime_error If slices have inconsistent dimensions
      */
     bool loadFromDirectory(const std::string& directoryPath, const std::string& extension = "png");
 
     /**
      * @brief Get a pixel (voxel) at a specific position
      * 
      * @param x X-coordinate
      * @param y Y-coordinate
      * @param z Z-coordinate
      * @return Pixel The pixel at the specified position
      * @throws std::out_of_range If coordinates are out of bounds
      */
     Pixel getVoxel(int x, int y, int z) const;
 
     /**
      * @brief Set a pixel (voxel) at a specific position
      * 
      * @param x X-coordinate
      * @param y Y-coordinate
      * @param z Z-coordinate
      * @param pixel The pixel to set
      * @throws std::out_of_range If coordinates are out of bounds
      */
     void setVoxel(int x, int y, int z, const Pixel& pixel);
 
     /**
      * @brief Save all slices of the volume to files
      * 
      * @param baseFilename Base filename to use (will be appended with slice number)
      * @param extension File extension to use (e.g., ".png")
      * @return bool True if saving was successful, false otherwise
      */
     bool saveToFiles(const std::string& baseFilename, const std::string& extension = ".png") const;
 
     /**
      * @brief Save a specific slice of the volume to a file
      * 
      * @param filename The name of the file to save to
      * @param sliceIndex The index of the slice to save
      * @return bool True if the operation was successful, false otherwise
      * @throws std::out_of_range If slice index is out of bounds
      */
     bool saveSliceToFile(const std::string& filename, int sliceIndex) const;
 
     /**
      * @brief Apply a filter to the volume
      * 
      * Delegates filtering operation to the VolumeFilter object.
      * 
      * @param filter The volume filter to apply
      * @return std::unique_ptr<Volume> A new volume with the filter applied
      */
     std::unique_ptr<Volume> applyFilter(VolumeFilter& filter) const;
 
     /**
      * @brief Apply a Gaussian 3D filter to the volume
      * 
      * @param kernelSize Size of the kernel (must be odd, e.g., 3, 5, 7)
      * @param sigma Standard deviation for the Gaussian filter
      * @return std::unique_ptr<Volume> A new volume with the filter applied
      */
     std::unique_ptr<Volume> applyGaussianFilter(int kernelSize = 3, float sigma = 2.0f) const;
 
     /**
      * @brief Apply a Median 3D filter to the volume
      * 
      * @param kernelSize Size of the kernel (must be odd, e.g., 3, 5, 7)
      * @return std::unique_ptr<Volume> A new volume with the filter applied
      */
     std::unique_ptr<Volume> applyMedianFilter(int kernelSize = 3) const;
 
     /**
      * @brief Apply a projection to generate a 2D image
      * 
      * Delegates projection operation to the Projection object, which could be:
      * - MaxIntensityProj for maximum intensity projection
      * - MinIntensityProj for minimum intensity projection
      * - AvgIntensityProj for average intensity projection (with mean or median)
      * 
      * @param projection The projection to apply
      * @return Image The resulting 2D projection
      */
     Image applyProjection(const Projection& projection) const;
 
     /**
      * @brief Extract a slice from the volume
      * 
      * Delegates slice extraction to the Slice object, which supports:
      * - XY slices (constant Z)
      * - YZ slices (constant X)
      * - XZ slices (constant Y)
      * 
      * @param slice The slice extractor
      * @return Image The extracted 2D slice
      */
     Image extractSlice(const Slice& slice) const;
 
     // Override methods from DataContainer
     Pixel getPixel(int x, int y) const override;
     void setPixel(int x, int y, const Pixel& pixel) override;
     std::unique_ptr<DataContainer> clone() const override;
     bool saveToFile(const std::string& filename) const override;
     int getChannels() const override;
     std::unique_ptr<DataContainer> applyFilter(const std::string& filterName,
                                              const std::string& parameters = "") const override;
 };
 
 #endif // VOLUME_H