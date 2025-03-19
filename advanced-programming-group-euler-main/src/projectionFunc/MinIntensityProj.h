/**
 * @file MinIntensityProj.h
 * @brief Definition of the MinIntensityProj class
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

#ifndef MIN_INTENSITY_PROJ_H
#define MIN_INTENSITY_PROJ_H

#include "Projection.h"

/**
 * @brief Class for Minimum Intensity Projection (MinIP)
 * 
 * This class implements a minimum intensity projection, which finds the minimum
 * intensity of all voxels with a given (x, y) coordinate along the z-direction.
 */
class MinIntensityProj : public Projection {
public:
    /**
     * @brief Constructor
     * 
     * @param slabStart Starting Z-index for the projection (defaults to 0)
     * @param slabEnd Ending Z-index for the projection (defaults to -1, meaning the last slice)
     */
    MinIntensityProj(int slabStart = 0, int slabEnd = -1);

    /**
     * @brief Apply the minimum intensity projection to a volume
     * 
     * @param volume The volume to project
     * @return Image The resulting 2D projection
     */
    Image apply(const Volume& volume) const override;
};

#endif // MIN_INTENSITY_PROJ_H