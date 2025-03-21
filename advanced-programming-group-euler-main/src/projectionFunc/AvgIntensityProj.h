/**
 * @file AvgIntensityProj.h
 * @brief Definition of the AvgIntensityProj class
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

#ifndef AVG_INTENSITY_PROJ_H
#define AVG_INTENSITY_PROJ_H

#include "Projection.h"

/**
 * @brief Class for Average Intensity Projection (AIP)
 * 
 * This class implements an average intensity projection, which calculates
 * the average (mean) intensity of all voxels with a given (x, y) coordinate
 * along the z-direction.
 */
class AvgIntensityProj : public Projection {
private:
    bool useMedian; ///< If true, use median instead of mean

public:
    /**
     * @brief Constructor
     * 
     * @param slabStart Starting Z-index for the projection (defaults to 0)
     * @param slabEnd Ending Z-index for the projection (defaults to -1, meaning the last slice)
     * @param useMedian If true, calculate median intensity instead of mean (defaults to false)
     */
    AvgIntensityProj(int slabStart = 0, int slabEnd = -1, bool useMedian = false);

    /**
     * @brief Set whether to use median instead of mean
     * 
     * @param useMedian True to use median, false to use mean
     */
    void setUseMedian(bool useMedian);

    /**
     * @brief Check if using median instead of mean
     * 
     * @return bool True if using median, false if using mean
     */
    bool isUsingMedian() const;

    /**
     * @brief Apply the average intensity projection to a volume
     * 
     * @param volume The volume to project
     * @return Image The resulting 2D projection
     */
    Image apply(const Volume& volume) const override;
};

#endif // AVG_INTENSITY_PROJ_H