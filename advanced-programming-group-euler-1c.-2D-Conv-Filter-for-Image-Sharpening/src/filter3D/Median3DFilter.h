/**
 * @file Median3DFilter.h
 * @brief Definition of the Median3DFilter class
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

 #ifndef MEDIAN_3D_FILTER_H
 #define MEDIAN_3D_FILTER_H
 
 #include "VolumeFilter.h"
 
 /**
  * @brief Class for applying a 3D median filter to a volume
  * 
  * This filter replaces each voxel with the median value of its neighborhood,
  * effectively removing noise while preserving edges.
  */
 class Median3DFilter : public VolumeFilter {
 public:
     /**
      * @brief Constructor with kernel size
      * 
      * @param kernelSize Size of the kernel (must be odd, e.g., 3, 5, 7)
      * @throws std::invalid_argument If kernel size is not odd
      */
     Median3DFilter(int kernelSize = 3);
 
     /**
      * @brief Apply the median filter to a volume
      * 
      * @param volume The volume to apply the filter to
      * @return std::unique_ptr<Volume> A new volume with the filter applied
      */
     std::unique_ptr<Volume> apply(const Volume& volume) const override;
 };
 
 #endif // MEDIAN_3D_FILTER_H