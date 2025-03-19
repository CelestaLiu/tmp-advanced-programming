/**
 * @file Slice.h
 * @brief Definition of the Slice class for extracting 2D slices from 3D volumes
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

 #ifndef SLICE_H
 #define SLICE_H
 
 // Forward declarations
 class Volume;
 class Image;
 
 /**
  * @brief Enumeration for slice plane types
  */
 enum class SlicePlane {
     XY, ///< Slice in the X-Y plane (constant Z)
     XZ, ///< Slice in the X-Z plane (constant Y)
     YZ  ///< Slice in the Y-Z plane (constant X)
 };
 
 /**
  * @brief Class for extracting 2D slices from 3D volumes
  * 
  * The Slice class enables extraction of 2D planar slices from 3D volumes.
  * Slices can be taken along any of the three principal planes: XY, XZ, or YZ.
  */
 class Slice {
 private:
     SlicePlane plane;     ///< Which plane to slice along
     int position;         ///< Position along the constant axis (e.g., Z-position for XY plane)
 
 public:
     /**
      * @brief Default constructor - creates an XY slice at position 0
      */
     Slice();
 
     /**
      * @brief Constructor with slice plane and position
      * 
      * @param plane Which plane to slice along
      * @param position Position along the constant axis (1-based index in user input, converted to 0-based index internally)
      */
     Slice(SlicePlane plane, int position);
 
     /**
      * @brief Get the slice plane
      * 
      * @return SlicePlane The current slice plane
      */
     SlicePlane getPlane() const;
 
     /**
      * @brief Get the slice position
      * 
      * @return int The current slice position
      */
     int getPosition() const;
 
     /**
      * @brief Set the slice plane
      * 
      * @param plane The new slice plane
      */
     void setPlane(SlicePlane plane);
 
     /**
      * @brief Set the slice position
      * 
      * @param position The new slice position (0-based index)
      */
     void setPosition(int position);
 
     /**
      * @brief Extract a 2D slice from the given volume
      * 
      * @param volume The volume to extract a slice from
      * @return Image The extracted 2D slice
      * @throws std::out_of_range If the slice position is outside the volume bounds
      */
     Image extract(const Volume& volume) const;
 };
 
 #endif // SLICE_H