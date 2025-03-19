/**
 * @file Projection.h
 * @brief Definition of the abstract Projection class
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

#ifndef PROJECTION_H
#define PROJECTION_H

#include <string>

// Forward declarations
class Volume;
class Image;

/**
 * @brief Enumeration for projection types
 */
enum class ProjectionType {
    MAXIMUM_INTENSITY,
    MINIMUM_INTENSITY,
    AVERAGE_INTENSITY
};

/**
 * @brief Abstract base class for volume projections
 * 
 * Projection provides an interface for creating 2D projections from a 3D volume.
 * Different types of projections (maximum, minimum, average intensity) can be
 * implemented as derived classes.
 */
class Projection {
protected:
    ProjectionType type; ///< Type of projection
    int slabStart;       ///< Starting Z-index for the projection
    int slabEnd;         ///< Ending Z-index for the projection

public:
    /**
     * @brief Constructor with projection type
     * 
     * @param type Type of projection
     * @param slabStart Starting Z-index for the projection (defaults to 0)
     * @param slabEnd Ending Z-index for the projection (defaults to -1, meaning the last slice)
     */
    Projection(ProjectionType type, int slabStart = 0, int slabEnd = -1);
    
    /**
     * @brief Virtual destructor
     */
    virtual ~Projection();
    
    /**
     * @brief Get the projection type
     * 
     * @return ProjectionType The type of projection
     */
    ProjectionType getType() const;
    
    /**
     * @brief Set the slab range for the projection
     * 
     * @param start Starting Z-index (inclusive)
     * @param end Ending Z-index (inclusive)
     */
    void setSlabRange(int start, int end);
    
    /**
     * @brief Apply the projection to a volume
     * 
     * @param volume The volume to project
     * @return Image The resulting 2D projection
     */
    virtual Image apply(const Volume& volume) const = 0;
};

#endif // PROJECTION_H