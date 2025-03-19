#include "../src/projectionFunc/AvgIntensityProj.h"
#include "../src/Volume.h"
#include "../src/Pixel.h"
#include "../src/Image.h"
#include "TestCounters.h"
#include <iostream>
#include <vector>



void test_mean_projection() {
    // Test 1: Uniform values (mean = value)
    {
        Volume vol(2, 2, 3, 3, "test");
        for (int z = 0; z < 3; z++) {
            for (int y = 0; y < 2; y++) {
                for (int x = 0; x < 2; x++) {
                    vol.setVoxel(x, y, z, Pixel(100, 0, 0));  // R=100, G=0, B=0
                }
            }
        }

        AvgIntensityProj proj(0, -1, false);  // Use mean
        Image result = proj.apply(vol);
        for (int y = 0; y < 2; y++) {
            for (int x = 0; x < 2; x++) {
                CHECK(result.getPixel(x, y) == Pixel(100, 0, 0), 
                    "Mean projection with uniform values");
            }
        }
    }

    // Test 2: Varying values (mean calculation)
    {
        Volume vol(1, 1, 3, 3, "test");
        vol.setVoxel(0, 0, 0, Pixel(90, 0, 0));   // R=90
        vol.setVoxel(0, 0, 1, Pixel(100, 0, 0));  // R=100
        vol.setVoxel(0, 0, 2, Pixel(110, 0, 0));  // R=110

        AvgIntensityProj proj(0, -1, false);
        Image result = proj.apply(vol);
        // (90 + 100 + 110) / 3 = 100
        CHECK(result.getPixel(0, 0) == Pixel(100, 0, 0), 
            "Mean projection with varying values");
    }
}

void test_median_projection() {
    // Test 1: Odd number of slices
    {
        Volume vol(1, 1, 5, 3, "test");
        vol.setVoxel(0, 0, 0, Pixel(50, 0, 0));   // R=50
        vol.setVoxel(0, 0, 1, Pixel(150, 0, 0));  // R=150
        vol.setVoxel(0, 0, 2, Pixel(100, 0, 0));  // R=100
        vol.setVoxel(0, 0, 3, Pixel(200, 0, 0));  // R=200
        vol.setVoxel(0, 0, 4, Pixel(10, 0, 0));   // R=10

        AvgIntensityProj proj(0, -1, true);  // Use median
        Image result = proj.apply(vol);
        // Sorted values: 10, 50, 100, 150, 200 → median = 100
        CHECK(result.getPixel(0, 0) == Pixel(100, 0, 0), 
            "Median projection (odd slices)");
    }

    // Test 2: Even number of slices (mean value for the middle two element)
    {
        Volume vol(1, 1, 4, 3, "test");
        vol.setVoxel(0, 0, 0, Pixel(30, 0, 0));  // R=30
        vol.setVoxel(0, 0, 1, Pixel(10, 0, 0));  // R=10
        vol.setVoxel(0, 0, 2, Pixel(40, 0, 0));  // R=40
        vol.setVoxel(0, 0, 3, Pixel(20, 0, 0));  // R=20

        AvgIntensityProj proj(0, -1, true);
        Image result = proj.apply(vol);
        // Sorted values: 10, 20, 30, 40 → mid = 20, 30 → mean = 25
        CHECK(result.getPixel(0, 0) == Pixel(25, 0, 0), 
            "Median projection (even slices)");
    }
}

void test_slab_range() {
    Volume vol(1, 1, 5, 3, "test");
    for (int z = 0; z < 5; z++) {
        vol.setVoxel(0, 0, z, Pixel(z * 50, 0, 0));  // R=0,50,100,150,200
    }

    // Test partial slab (Z1-Z3: 50,100,150)
    AvgIntensityProj proj(1, 3, false);
    Image result = proj.apply(vol);
    unsigned char expectedMean = (50 + 100 + 150) / 3;  // 100
    CHECK(result.getPixel(0, 0).getR() == expectedMean, 
        "Partial slab mean projection");
}

void test_use_median_flag() {
    Volume vol(1, 1, 3, 3, "test");
    vol.setVoxel(0, 0, 0, Pixel(0, 0, 0));
    vol.setVoxel(0, 0, 1, Pixel(0, 0, 0));
    vol.setVoxel(0, 0, 2, Pixel(30, 0, 0));

    // Test flag toggling
    AvgIntensityProj proj(0, -1, false);
    Image meanResult = proj.apply(vol);
    CHECK(meanResult.getPixel(0, 0) == Pixel(10, 0, 0), "Mean when useMedian=false");

    proj.setUseMedian(true);
    Image medianResult = proj.apply(vol);
    CHECK(medianResult.getPixel(0, 0) == Pixel(0, 0, 0), "Median when useMedian=true");
}


void runAvgIntensityProjTests() {
    std::cout << "\n=== Running AvgIntensityProj Tests ===\n";
    test_mean_projection();
    test_median_projection();
    test_slab_range();
    test_use_median_flag();
    // std::cout << "\nAvgIntensityProj Tests Summary: " 
    //             << passed << " passed, " << failed << " failed\n";
}