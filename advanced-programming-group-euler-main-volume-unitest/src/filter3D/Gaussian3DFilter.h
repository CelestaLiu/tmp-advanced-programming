/**
 * @file Gaussian3DFilter.h
 * @brief Definition of the Gaussian3DFilter class
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

 #ifndef GAUSSIAN_3D_FILTER_H
 #define GAUSSIAN_3D_FILTER_H
 
 #include "VolumeFilter.h"
 #include <vector>
 
 /**
  * @brief Class for applying a 3D Gaussian blur filter to a volume
  * 
  * This filter applies a 3D Gaussian blur by convolving the volume with a
  * 3D Gaussian kernel. The standard deviation (sigma) of the Gaussian
  * distribution can be configured.
  */
 class Gaussian3DFilter : public VolumeFilter {
 private:
     float sigma;                    ///< Standard deviation for the Gaussian filter
     std::vector<float> kernel;      ///< Precomputed 3D Gaussian kernel flattened to 1D
 
     /**
      * @brief Generate the 3D Gaussian kernel
      * 
      * Computes the weights for the 3D Gaussian kernel using the formula:
      * G(x,y,z) = (1/(2*π*σ²)^(3/2)) * e^(-(x²+y²+z²)/(2*σ²))
      * 
      * @return std::vector<float> Flattened 3D kernel array
      */
     std::vector<float> generateKernel() const;
 
 public:
     /**
      * @brief Constructor with kernel size and sigma
      * 
      * @param kernelSize Size of the kernel (must be odd, e.g., 3, 5, 7)
      * @param sigma Standard deviation for the Gaussian filter (default: 2.0)
      * @throws std::invalid_argument If kernel size is not odd
      */
     Gaussian3DFilter(int kernelSize = 3, float sigma = 2.0f);
 
     /**
      * @brief Get the standard deviation (sigma) of the Gaussian filter
      * 
      * @return float The sigma value
      */
     float getSigma() const;
 
     /**
      * @brief Set the standard deviation (sigma) of the Gaussian filter
      * 
      * @param sigma The new sigma value
      */
     void setSigma(float sigma);
 
     /**
      * @brief Apply the Gaussian blur filter to a volume
      * 
      * @param volume The volume to apply the filter to
      * @return std::unique_ptr<Volume> A new volume with the filter applied
      */
     std::unique_ptr<Volume> apply(const Volume& volume) const override;
 };
 
 #endif // GAUSSIAN_3D_FILTER_H