#include "../src/projectionFunc/MinIntensityProj.h"
#include "../src/Volume.h"
#include "../src/Pixel.h"
#include "../src/Image.h"
#include "TestCounters.h"
#include <iostream>



void test_min_projection_logic() {
    Volume vol(2, 2, 3, 1, "test");
    // Z0: 50, Z1: 150, Z2: 100
    vol.setVoxel(0, 0, 0, Pixel(50, 0, 0));
    vol.setVoxel(0, 0, 1, Pixel(150, 0, 0));
    vol.setVoxel(0, 0, 2, Pixel(100, 0, 0));

    // Test full volume max
    {
        MinIntensityProj proj(0, -1);
        Image result = proj.apply(vol);
        CHECK(result.getPixel(0, 0).getR() == 50, "Correct min intensity");
    }

    // Test partial slab (Z0-Z1)
    {
        MinIntensityProj proj(0, 1);
        Image result = proj.apply(vol);
        CHECK(result.getPixel(0, 0).getR() == 50, "Partial slab min");
    }
}


void runMinIntensityProjTests() {
    std::cout << "\n=== Running MinIntensityProj Tests ===\n";
    test_min_projection_logic();
}