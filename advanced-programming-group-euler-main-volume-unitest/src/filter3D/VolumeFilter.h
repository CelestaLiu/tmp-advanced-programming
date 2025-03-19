/**
 * @file VolumeFilter.h
 * @brief Definition of the abstract VolumeFilter class
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

 #ifndef VOLUME_FILTER_H
 #define VOLUME_FILTER_H
 
 #include <memory>
 #include <string>
 
 // Forward declaration
 class Volume;
 
 /**
  * @brief Abstract base class for 3D volume filters
  * 
  * This class provides an interface for implementing different 3D filters
  * like Gaussian blur and median filter that can be applied to a volume.
  */
 class VolumeFilter {
 protected:
     std::string name; ///< Name of the filter
     int kernelSize;   ///< Size of the filter kernel (should be odd)
 
 public:
     /**
      * @brief Constructor with filter name and kernel size
      * 
      * @param name Name of the filter
      * @param kernelSize Size of the filter kernel (must be odd)
      * @throws std::invalid_argument If kernel size is not odd
      */
     VolumeFilter(const std::string& name, int kernelSize);
     
     /**
      * @brief Virtual destructor
      */
     virtual ~VolumeFilter();
 
     /**
      * @brief Get the filter name
      * 
      * @return std::string The filter name
      */
     std::string getName() const;
 
     /**
      * @brief Get the kernel size
      * 
      * @return int The kernel size
      */
     int getKernelSize() const;
 
     /**
      * @brief Set the kernel size
      * 
      * @param kernelSize The new kernel size (must be odd)
      * @throws std::invalid_argument If kernel size is not odd
      */
     void setKernelSize(int kernelSize);
 
     /**
      * @brief Apply the filter to a volume
      * 
      * This is a pure virtual method that must be implemented by derived classes.
      * 
      * @param volume The volume to apply the filter to
      * @return std::unique_ptr<Volume> A new volume with the filter applied
      */
     virtual std::unique_ptr<Volume> apply(const Volume& volume) const = 0;
 };
 
 #endif // VOLUME_FILTER_H