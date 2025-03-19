#include "../src/Volume.h"
#include "../src/Pixel.h"
#include "../src/filter3D/Gaussian3DFilter.h"
#include "TestCounters.h"
#include <iostream>
#include <cstdlib>
#include <vector>
#include <filesystem>
#include <stdexcept>

namespace fs = std::filesystem;

// Test result counters
// int passed = 0;
// int failed = 0;

#define CHECK(condition, message) \
    do { \
        if (!(condition)) { \
            std::cerr << "\033[1;31m[FAIL]\033[0m " << message << " (" << __FILE__ << ":" << __LINE__ << ")\n"; \
            failed++; \
        } else { \
            std::cout << "\033[1;32m[PASS]\033[0m " << message << "\n"; \
            passed++; \
        } \
    } while(0)


// Utility macro for exception checking
#define CHECK_THROWS(expression, message) \
    do { \
        bool threw = false; \
        try { expression; } \
        catch (...) { threw = true; } \
        CHECK(threw, message); \
    } while(0)


void test_constructors() {
    // Test valid construction
    try {
        Volume vol(128, 256, 64, 3, "valid");
        CHECK(vol.getWidth() == 128, "Width correct");
        CHECK(vol.getHeight() == 256, "Height correct");
        CHECK(vol.getDepth() == 64, "Depth correct");
        CHECK(vol.getChannels() == 3, "Channels correct");
        
        // Test default voxel values
        Pixel p = vol.getVoxel(0, 0, 0);
        CHECK(p.getR() == 0 && p.getG() == 0 && p.getB() == 0, "Default voxel is black");
    } catch (...) {
        CHECK(false, "Valid constructor threw unexpected exception");
    }

    // Test invalid parameters
    CHECK_THROWS(Volume(-1, 10, 10, 1, "invalid"), "Negative width throws");
    CHECK_THROWS(Volume(10, 10, -5, 1, "invalid"), "Negative depth throws");
    CHECK_THROWS(Volume(10, 10, 5, 5, "invalid"), "Invalid channels throws");
}

void test_voxel_operations() {
    Volume vol(10, 10, 10, 4, "test");
    
    // Test valid set/get
    Pixel testPixel(255, 128, 64, 200);
    vol.setVoxel(5, 5, 5, testPixel);
    Pixel retrieved = vol.getVoxel(5, 5, 5);
    CHECK(retrieved == testPixel, "Voxel set/get matches");

    // Test bounds checking
    CHECK_THROWS(vol.getVoxel(10, 5, 5), "X out of bounds throws");
    CHECK_THROWS(vol.setVoxel(5, 10, 5, testPixel), "Y out of bounds throws");
    CHECK_THROWS(vol.getVoxel(5, 5, 10), "Z out of bounds throws");
}

void test_file_io() {
    // Create test directory with sample slices
    fs::path testDir = "test_volume";
    fs::create_directory(testDir);
    
    // Create 3 test slices
    for(int z = 0; z < 3; z++) {
        Volume sliceVol(2, 2, 1, 3, "slice");
        sliceVol.setVoxel(0, 0, 0, Pixel(z*100, z*100, z*100));
        sliceVol.saveToFile((testDir / ("slice_" + std::to_string(z) + ".png")).string());
    }

    // Test loading from directory
    try {
        Volume loadedVol(testDir.string(), "png", "loaded");
        CHECK(loadedVol.getDepth() == 3, "Loaded correct depth");
        CHECK(loadedVol.getVoxel(0, 0, 1).getR() == 100, "Slice data correct");
    } catch (...) {
        CHECK(false, "Directory load failed");
    }

    // Cleanup
    fs::remove_all(testDir);
}

// void test_cloning() {
//     Volume original(5, 5, 5, 4, "original");
//     original.setVoxel(2, 2, 2, Pixel(255, 0, 0));
    
//     auto clone = original.clone();
//     // Cast to Volume* to access Volume-specific methods
//     Volume* clonedVolume = static_cast<Volume*>(clone.get());
//     CHECK(clone->getVoxel(2, 2, 2).getR() == 255, "Clone matches original");
// }
void test_filtering() {
    Volume vol(5, 5, 5, 1, "test");
    vol.setVoxel(2, 2, 2, Pixel(255, 0, 0));
    
    Gaussian3DFilter filter(3, 1.0f);
    auto filtered = filter.apply(vol);
    
    // Check filtering effect
    Pixel center = filtered->getVoxel(2, 2, 2);
    CHECK(center.getR() < 255, "Gaussian reduces center intensity");
    CHECK(filtered->getVoxel(2, 2, 1).getR() > 0, "Gaussian creates blur effect");
}

void runVolumeTests() {
    std::cout << "\n=== Running Volume Tests ===\n";
    
    test_constructors();
    test_voxel_operations();
    test_file_io();
    // test_cloning();
    test_filtering();
    
    // std::cout << "\nVolume Tests Summary: "
    //           << passed << " passed, " << failed << " failed\n";
}

