/**
 * @file MinIntensityProj.h
 * @brief Definition of the MinIntensityProj class
 * @group [GROUP_NAME]
 * 
 * Group members:
 * - [MEMBER1] ([USERNAME1])
 * - [MEMBER2] ([USERNAME2])
 * - [MEMBER3] ([USERNAME3])
 * - [MEMBER4] ([USERNAME4])
 * - [MEMBER5] ([USERNAME5])
 * - [MEMBER6] ([USERNAME6])
 * - [MEMBER7] ([USERNAME7])
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