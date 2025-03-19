#include "../src/projectionFunc/MaxIntensityProj.h"
#include "../src/Volume.h"
#include "../src/Pixel.h"
#include "../src/Image.h"
#include "TestCounters.h"
#include <iostream>



void test_max_intensity_threshold() {
    Volume vol(3, 3, 3, 1, "test");
    for (int z = 0; z < 3; z++) {
        for (int y = 0; y < 3; y++) {
            for (int x = 0; x < 3; x++) {
                vol.setVoxel(x, y, z, Pixel(z * 100, 0, 0));  // Z0:0, Z1:100, Z2:200
            }
        }
    }

    // Test threshold filtering
    MaxIntensityProj proj(0, -1);
    proj.setThreshold(200.0f * 0.21f);  // 200 * 0.21 = 42, only Z2 should pass
    Image result = proj.apply(vol);
    
    // Only Z2 (200) should pass the threshold
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            CHECK(result.getPixel(x, y).getR() == 200, "Threshold filtering works");
        }
    }
}

void test_max_projection_logic() {
    Volume vol(2, 2, 3, 1, "test");
    // Z0: 50, Z1: 150, Z2: 100
    vol.setVoxel(0, 0, 0, Pixel(50, 0, 0));
    vol.setVoxel(0, 0, 1, Pixel(150, 0, 0));
    vol.setVoxel(0, 0, 2, Pixel(100, 0, 0));

    // Test full volume max
    {
        MaxIntensityProj proj(0, -1);
        Image result = proj.apply(vol);
        CHECK(result.getPixel(0, 0).getR() == 150, "Correct max intensity");
    }

    // Test partial slab (Z0-Z1)
    {
        MaxIntensityProj proj(0, 1);
        Image result = proj.apply(vol);
        CHECK(result.getPixel(0, 0).getR() == 150, "Partial slab max");
    }
}

void test_empty_projection() {
    Volume vol(2, 2, 2, 1, "test");
    MaxIntensityProj proj(0, -1);
    proj.setThreshold(200.0f);  // No voxels meet threshold
    
    Image result = proj.apply(vol);
    CHECK(result.getPixel(0, 0).getR() == 0, "Black pixel when no voxels pass threshold");
}

void runMaxIntensityProjTests() {
    std::cout << "\n=== Running MaxIntensityProj Tests ===\n";
    test_max_intensity_threshold();
    test_max_projection_logic();
    test_empty_projection();
    // std::cout << "\nMaxIntensityProj Tests Summary: " << passed << " passed, " << failed << " failed\n";
}