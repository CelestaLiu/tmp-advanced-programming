// testSlice.cpp
#include "../src/Slice.h"
#include "../src/Volume.h"
#include "../src/Pixel.h"
#include "../src/Image.h"
#include "TestCounters.h"
#include <iostream>
#include <stdexcept>

void test_constructor_position_conversion() {
    // Test 1-based to 0-based conversion
    Slice slice1(SlicePlane::XY, 1);
    CHECK(slice1.getPosition() == 0, "Position 1 converts to 0");

    Slice slice2(SlicePlane::XY, 5);
    CHECK(slice2.getPosition() == 4, "Position 5 converts to 4");
}

void test_getters_setters() {
    Slice slice;
    slice.setPlane(SlicePlane::XZ);
    slice.setPosition(3);
    CHECK(slice.getPlane() == SlicePlane::XZ, "Plane set correctly");
    CHECK(slice.getPosition() == 3, "Position set correctly");
}

void test_extract_xy_slice() {
    // Create volume with distinct Z-layer colors
    Volume vol(3, 3, 3, 3, "test");
    for (int z = 0; z < 3; z++) {
        Pixel color;
        switch (z) {
            case 0: color = Pixel(255, 0, 0); break; // Red
            case 1: color = Pixel(0, 255, 0); break; // Green
            case 2: color = Pixel(0, 0, 255); break; // Blue
        }
        for (int y = 0; y < 3; y++) {
            for (int x = 0; x < 3; x++) {
                vol.setVoxel(x, y, z, color);
            }
        }
    }

    Slice slice(SlicePlane::XY, 2); // Z=1 (0-based)
    Image img = slice.extract(vol);
    
    // Verify all pixels are green
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            CHECK(img.getPixel(x, y) == Pixel(0, 255, 0), "XY slice at Z=1 is green");
        }
    }
}

void test_extract_xz_slice() {
    // Create volume with distinct Y-layer colors
    Volume vol(3, 3, 3, 3, "test");
    for (int y = 0; y < 3; y++) {
        Pixel color;
        switch (y) {
            case 0: color = Pixel(255, 0, 0); break; // Red
            case 1: color = Pixel(0, 255, 0); break; // Green
            case 2: color = Pixel(0, 0, 255); break; // Blue
        }
        for (int z = 0; z < 3; z++) {
            for (int x = 0; x < 3; x++) {
                vol.setVoxel(x, y, z, color);
            }
        }
    }

    Slice slice(SlicePlane::XZ, 2); // Y=1 (0-based)
    Image img = slice.extract(vol);
    
    // Verify all pixels are green
    for (int z = 0; z < 3; z++) {
        for (int x = 0; x < 3; x++) {
            CHECK(img.getPixel(x, z) == Pixel(0, 255, 0), "XZ slice at Y=1 is green");
        }
    }
}

void test_extract_yz_slice() {
    // Create volume with distinct X-layer colors
    Volume vol(3, 3, 3, 3, "test");
    for (int x = 0; x < 3; x++) {
        Pixel color;
        switch (x) {
            case 0: color = Pixel(255, 0, 0); break; // Red
            case 1: color = Pixel(0, 255, 0); break; // Green
            case 2: color = Pixel(0, 0, 255); break; // Blue
        }
        for (int z = 0; z < 3; z++) {
            for (int y = 0; y < 3; y++) {
                vol.setVoxel(x, y, z, color);
            }
        }
    }

    Slice slice(SlicePlane::YZ, 2); // X=1 (0-based)
    Image img = slice.extract(vol);
    
    // Verify all pixels are green
    for (int z = 0; z < 3; z++) {
        for (int y = 0; y < 3; y++) {
            CHECK(img.getPixel(y, z) == Pixel(0, 255, 0), "YZ slice at X=1 is green");
        }
    }
}

void test_boundary_positions() {
    Volume vol(4, 5, 6, 3, "test");
    
    // Test max valid positions
    CHECK_NOTHROW(Slice(SlicePlane::XY, 6).extract(vol), "Max XY position (Z=5)"); // depth=6 → user input 6 (0-based 5)
    CHECK_NOTHROW(Slice(SlicePlane::XZ, 5).extract(vol), "Max XZ position (Y=4)"); // height=5 → user input 5 (0-based 4)
    CHECK_NOTHROW(Slice(SlicePlane::YZ, 4).extract(vol), "Max YZ position (X=3)"); // width=4 → user input 4 (0-based 3)
}

void test_invalid_positions() {
    Volume vol(3, 3, 3, 3, "test");
    
    // XY plane (Z-axis)
    CHECK_THROWS(Slice(SlicePlane::XY, 0).extract(vol), "Z position -1 throws"); // user input 0 → -1
    CHECK_THROWS(Slice(SlicePlane::XY, 4).extract(vol), "Z position 3 throws");  // depth=3 → max Z=2
    
    // XZ plane (Y-axis)
    CHECK_THROWS(Slice(SlicePlane::XZ, 0).extract(vol), "Y position -1 throws");
    CHECK_THROWS(Slice(SlicePlane::XZ, 4).extract(vol), "Y position 3 throws");
    
    // YZ plane (X-axis)
    CHECK_THROWS(Slice(SlicePlane::YZ, 0).extract(vol), "X position -1 throws");
    CHECK_THROWS(Slice(SlicePlane::YZ, 4).extract(vol), "X position 3 throws");
}

void test_image_dimensions() {
    Volume vol(4, 5, 6, 3, "test");
    
    Slice sliceXY(SlicePlane::XY, 1);
    Image imgXY = sliceXY.extract(vol);
    CHECK(imgXY.getWidth() == 4 && imgXY.getHeight() == 5, "XY slice dimensions 4x5");

    Slice sliceXZ(SlicePlane::XZ, 1);
    Image imgXZ = sliceXZ.extract(vol);
    CHECK(imgXZ.getWidth() == 4 && imgXZ.getHeight() == 6, "XZ slice dimensions 4x6");

    Slice sliceYZ(SlicePlane::YZ, 1);
    Image imgYZ = sliceYZ.extract(vol);
    CHECK(imgYZ.getWidth() == 5 && imgYZ.getHeight() == 6, "YZ slice dimensions 5x6");
}

void runSliceTests() {
    std::cout << "\n=== Running Slice Tests ===\n";
    test_constructor_position_conversion();
    test_getters_setters();
    test_extract_xy_slice();
    test_extract_xz_slice();
    test_extract_yz_slice();
    test_boundary_positions();
    test_invalid_positions();
    test_image_dimensions();
}