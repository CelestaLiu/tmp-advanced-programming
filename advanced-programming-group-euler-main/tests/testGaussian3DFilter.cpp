#include "../src/filter3D/Gaussian3DFilter.h"
#include "../src/Volume.h"
#include "../src/Pixel.h"
#include "TestCounters.h"
#include <cmath>
#include <iostream>
#include <memory>
#include <tuple>

void test_constructor_validation() {
    // Test valid kernel sizes
    CHECK_NOTHROW(Gaussian3DFilter(3, 1.0f), "Valid kernel size 3");
    CHECK_NOTHROW(Gaussian3DFilter(5, 2.5f), "Valid kernel size 5");
    
    // Test invalid kernel sizes
    CHECK_THROWS(Gaussian3DFilter(4, 1.0f), "Even kernel size throws");
    CHECK_THROWS(Gaussian3DFilter(-3, 1.0f), "Negative kernel size throws");
}

void test_sigma_operations() {
    Gaussian3DFilter filter(3, 2.0f);
    CHECK(filter.getSigma() == 2.0f, "Initial sigma correct");
    
    filter.setSigma(1.5f);
    CHECK(filter.getSigma() == 1.5f, "Sigma updated correctly");
    
    CHECK_THROWS(filter.setSigma(-1.0f), "Negative sigma throws");
}

// void test_kernel_generation() {
//     // Test kernel normalization
//     {
//         Gaussian3DFilter filter(3, 1.0f);
//         auto kernel = filter.generateKernel();
        
//         float sum = 0.0f;
//         for (float val : kernel) sum += val;
//         CHECK(std::abs(sum - 1.0f) < 0.001f, "Kernel sums to 1");
//     }

//     // Test kernel dimensions
//     {
//         Gaussian3DFilter filter(5, 2.0f);
//         auto kernel = filter.generateKernel();
//         CHECK(kernel.size() == 5*5*5, "Kernel has correct size");
//     }

//     // Test kernel symmetry and peak
//     {
//         Gaussian3DFilter filter(3, 1.0f);
//         auto kernel = filter.generateKernel();
        
//         // Center should be maximum value
//         float center = kernel[13]; // (1,1,1) in 3x3x3
//         float corner = kernel[0];   // (-1,-1,-1)
//         CHECK(center > corner, "Center value > corner");
//     }
// }

void test_constant_volume() {
    Volume vol(5, 5, 5, 4, "ConstantVolume");
    for (int z = 0; z < 5; z++) {
        for (int y = 0; y < 5; y++) {
            for (int x = 0; x < 5; x++) {
                vol.setVoxel(x, y, z, Pixel(100, 100, 100, 255));
            }
        }
    }

    Gaussian3DFilter filter(3, 2.0f);
    auto filtered = filter.apply(vol);
    
    const float tolerance = 2.0f;
    for (int z = 1; z < 4; z++) {
        for (int y = 1; y < 4; y++) {
            for (int x = 1; x < 4; x++) {
                Pixel p = filtered->getVoxel(x, y, z);
                CHECK(std::abs(p.getR() - 100) <= tolerance, "R channel preserved");
                CHECK(std::abs(p.getG() - 100) <= tolerance, "G channel preserved");
                CHECK(std::abs(p.getB() - 100) <= tolerance, "B channel preserved");
                CHECK(p.getA() == 255, "Alpha preserved");
            }
        }
    }
}

void test_single_spike_volume() {
    Volume vol(5, 5, 5, 1, "SpikeVolume");
    vol.setVoxel(2, 2, 2, Pixel(255, 0, 0)); // Single bright voxel
    
    Gaussian3DFilter filter(3, 1.0f);
    auto filtered = filter.apply(vol);
    
    Pixel center = filtered->getVoxel(2, 2, 2);
    Pixel neighbor = filtered->getVoxel(2, 2, 1);
    
    CHECK(center.getR() < 255, "Center intensity reduced");
    CHECK(neighbor.getR() > 0, "Neighbor has spillover");
}

void test_boundary_handling() {
    Volume vol(3, 3, 3, 1, "BoundaryTest");
    vol.setVoxel(0, 0, 0, Pixel(255, 0, 0));
    
    Gaussian3DFilter filter(3, 1.0f);
    auto filtered = filter.apply(vol);
    
    // Should handle edges without crashing
    CHECK_NOTHROW(filtered->getVoxel(0, 0, 0), "Edge voxel accessible");
}

void runGaussian3DFilterTests() {
    std::cout << "\n=== Running Gaussian3DFilter Tests ===\n";
    test_constructor_validation();
    test_sigma_operations();
    // test_kernel_generation();
    test_constant_volume();
    test_single_spike_volume();
    test_boundary_handling();

}