// // tests/testMedian3DFilter.cpp
// #include "../src/filter3D/Median3DFilter.h"
// #include "../src/Volume.h"
// #include "../src/Pixel.h"
// #include "TestCounters.h"
// #include <cassert>
// #include <iostream>
// #include <memory>
// #include <tuple>
// #include <vector>

// void runMedian3DFilterTests() {
//     std::cout << "Running Median3DFilter tests..." << std::endl;

//     // Test 1: Constant Volume Filtering
//     {
//         // Create a constant volume (e.g., 5x5x5) with every voxel having R=150, G=150, B=150, A=255.
//         int width = 5, height = 5, depth = 5, channels = 4;
//         Volume vol(width, height, depth, channels, "TestVolume");
//         for (int z = 0; z < depth; ++z) {
//             for (int y = 0; y < height; ++y) {
//                 for (int x = 0; x < width; ++x) {
//                     vol.setVoxel(x, y, z, Pixel(150, 150, 150, 255));
//                 }
//             }
//         }
        
//         // Create a Median filter with kernel size 3.
//         Median3DFilter filter(3);
//         std::unique_ptr<Volume> filteredVol = filter.apply(vol);
        
//         // Since the volume is constant, the median should be the same.
//         for (int z = 0; z < depth; ++z) {
//             for (int y = 0; y < height; ++y) {
//                 for (int x = 0; x < width; ++x) {
//                     Pixel p = filteredVol->getVoxel(x, y, z);
//                     assert(p.getR() == 150);
//                     assert(p.getG() == 150);
//                     assert(p.getB() == 150);
//                     assert(p.getA() == 255);
//                 }
//             }
//         }
//         std::cout << "Test passed: Constant Volume Filtering" << std::endl;
//     }

//     // Test 2: Output Dimensions Preservation
//     {
//         int width = 3, height = 3, depth = 3, channels = 4;
//         Volume vol(width, height, depth, channels, "TestVolume");
//         // Fill the volume with a constant value.
//         for (int z = 0; z < depth; ++z) {
//             for (int y = 0; y < height; ++y) {
//                 for (int x = 0; x < width; ++x) {
//                     vol.setVoxel(x, y, z, Pixel(100, 100, 100, 255));
//                 }
//             }
//         }
        
//         Median3DFilter filter(3);
//         std::unique_ptr<Volume> filteredVol = filter.apply(vol);
//         int outWidth, outHeight, outDepth;
//         std::tie(outWidth, outHeight, outDepth) = filteredVol->getDimensions3D();
//         assert(outWidth == width && outHeight == height && outDepth == depth);
//         std::cout << "Test passed: Output volume dimensions are preserved" << std::endl;
//     }
    
//     std::cout << "All Median3DFilter tests passed!" << std::endl;
// }

// testMedian3DFilter.cpp
#include "../src/filter3D/Median3DFilter.h"
#include "../src/Volume.h"
#include "../src/Pixel.h"
#include "TestCounters.h"
#include <iostream>
#include <memory>
#include <vector>

void test_constructor_validation_1() {
    // Test valid kernel sizes
    CHECK_NOTHROW(Median3DFilter(3), "Valid kernel size 3");
    CHECK_NOTHROW(Median3DFilter(5), "Valid kernel size 5");
    
    // Test invalid kernel sizes
    CHECK_THROWS(Median3DFilter(4), "Even kernel size throws");
    CHECK_THROWS(Median3DFilter(-3), "Negative kernel size throws");
}

void test_constant_volume_1() {
    Volume vol(5, 5, 5, 4, "ConstantVolume");
    for (int z = 0; z < 5; z++) {
        for (int y = 0; y < 5; y++) {
            for (int x = 0; x < 5; x++) {
                vol.setVoxel(x, y, z, Pixel(100, 100, 100, 255));
            }
        }
    }

    Median3DFilter filter(3);
    auto filtered = filter.apply(vol);
    
    // All values should remain unchanged
    for (int z = 0; z < 5; z++) {
        for (int y = 0; y < 5; y++) {
            for (int x = 0; x < 5; x++) {
                Pixel p = filtered->getVoxel(x, y, z);
                CHECK(p == Pixel(100, 100, 100, 255), "Constant values preserved");
            }
        }
    }
}

void test_single_spike_removal() {
    Volume vol(5, 5, 5, 1, "SpikeVolume");
    // Set all to 50 except center
    for (int z = 0; z < 5; z++) {
        for (int y = 0; y < 5; y++) {
            for (int x = 0; x < 5; x++) {
                vol.setVoxel(x, y, z, Pixel(50, 0, 0));
            }
        }
    }
    vol.setVoxel(2, 2, 2, Pixel(255, 0, 0)); // Single spike
    
    Median3DFilter filter(3);
    auto filtered = filter.apply(vol);
    
    // Spike should be replaced with 50
    CHECK(filtered->getVoxel(2, 2, 2) == Pixel(50, 0, 0), "Single spike removed");
}

void test_median_calculation() {
    // Test with known median pattern
    Volume vol(3, 3, 3, 1, "MedianTest");
    int values[3][3][3] = {
        {{ 1,  2,  3},  { 4,  5,  6},  { 7,  8,  9}},
        {{10, 11, 99},  {13, 50, 15},  {16, 17, 18}},  // (1,1,1) is originally 50
        {{19, 20, 21},  {22, 23, 24},  {25, 26, 27}}
    };
    
    for (int z = 0; z < 3; z++) {
        for (int y = 0; y < 3; y++) {
            for (int x = 0; x < 3; x++) {
                vol.setVoxel(x, y, z, Pixel(values[z][y][x], 0, 0));
            }
        }
    }

    Median3DFilter filter(3);
    auto filtered = filter.apply(vol);
    
    // Center voxel's neighborhood contains all 27 values
    // Sorted median of all values
    CHECK(filtered->getVoxel(1, 1, 1) == Pixel(16, 0, 0), "Correct median calculation");
}

void test_edge_handling() {
    Volume vol(3, 3, 3, 1, "EdgeTest");
    vol.setVoxel(0, 0, 0, Pixel(255, 0, 0));
    
    Median3DFilter filter(3);
    auto filtered = filter.apply(vol);
    
    // Should handle edge voxel without crashing
    CHECK_NOTHROW(filtered->getVoxel(0, 0, 0), "Edge voxel processed");
    // Median of partial neighborhood (only valid voxels considered)
    CHECK(filtered->getVoxel(0, 0, 0).getR() < 255, "Edge spike reduced");
}

void test_multi_channel_support() {
    Volume vol(3, 3, 3, 4, "MultiChannel");
    // Set different patterns per channel
    for (int z = 0; z < 3; z++) {
        for (int y = 0; y < 3; y++) {
            for (int x = 0; x < 3; x++) {
                vol.setVoxel(x, y, z, Pixel(
                    x + y + z,        // R: 0-6
                    (x + y + z) * 10, // G: 0-60
                    (x + y + z) * 20, // B: 0-120
                    255               // A
                ));
            }
        }
    }

    Median3DFilter filter(3);
    auto filtered = filter.apply(vol);
    
    // Check center voxel medians
    Pixel p = filtered->getVoxel(1, 1, 1);
    CHECK(p.getR() == 3, "Red channel median");
    CHECK(p.getG() == 30, "Green channel median");
    CHECK(p.getB() == 60, "Blue channel median");
    CHECK(p.getA() == 255, "Alpha preserved");
}

void runMedian3DFilterTests() {
    std::cout << "\n=== Running Median3DFilter Tests ===\n";
    test_constructor_validation_1();
    test_constant_volume_1();
    test_single_spike_removal();
    test_median_calculation();
    test_edge_handling();
    test_multi_channel_support();
    // std::cout << "\nMedian3DFilter Tests Summary: "
    //           << passed << " passed, " << failed << " failed\n";
}