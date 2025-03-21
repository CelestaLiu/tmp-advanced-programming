/**
 * @file MaxIntensityProj.h
 * @brief Definition of the MaxIntensityProj class
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

#ifndef MAX_INTENSITY_PROJ_H
#define MAX_INTENSITY_PROJ_H

#include "Projection.h"

/**
 * @brief Class for Maximum Intensity Projection (MIP)
 * 
 * This class implements a maximum intensity projection, which finds the maximum
 * intensity of all voxels with a given (x, y) coordinate along the z-direction.
 */
class MaxIntensityProj : public Projection {
private:
    float threshold; ///< Optional threshold to filter out low intensity values (0-255)

public:
    /**
     * @brief Constructor
     * 
     * @param slabStart Starting Z-index for the projection (defaults to 0)
     * @param slabEnd Ending Z-index for the projection (defaults to -1, meaning the last slice)
     * @param threshold Optional intensity threshold (defaults to 0, meaning no thresholding)
     */
    MaxIntensityProj(int slabStart = 0, int slabEnd = -1, float threshold = 0.0f);

    /**
     * @brief Set the intensity threshold
     * 
     * @param threshold New threshold value (0-255)
     */
    void setThreshold(float threshold);

    /**
     * @brief Get the intensity threshold
     * 
     * @return float The current threshold value
     */
    float getThreshold() const;

    /**
     * @brief Apply the maximum intensity projection to a volume
     * 
     * @param volume The volume to project
     * @return Image The resulting 2D projection
     */
    Image apply(const Volume& volume) const override;
};

#endif // MAX_INTENSITY_PROJ_H