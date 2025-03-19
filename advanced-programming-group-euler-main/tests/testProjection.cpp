#include "../src/projectionFunc/Projection.h"
#include "../src/projectionFunc/MaxIntensityProj.h"  // Use for base class testing
#include "../src/Volume.h"
#include "TestCounters.h"
#include <iostream>


void runProjectionTests() {
    std::cout << "\n=== Running Projection Tests ===\n";

    // Test 1: Basic inheritance and type
    {
        MaxIntensityProj proj(0, 5);
        CHECK(proj.getType() == ProjectionType::MAXIMUM_INTENSITY, "Correct projection type");
    }

    // Test 2: Slab range validation (base class responsibility)
    {
        MaxIntensityProj proj(0, 5);
        
        // Valid cases
        CHECK_NOTHROW(proj.setSlabRange(2, 4), "Valid range accepted");
        CHECK_NOTHROW(proj.setSlabRange(3, -1), "-1 end index allowed");

        // Invalid cases
        CHECK_THROWS(proj.setSlabRange(5, 3), "Start > end throws");
        CHECK_THROWS(proj.setSlabRange(-2, 3), "Negative start throws");
    }

    // std::cout << "\nProjection Tests Summary: "
    //           << passed << " passed, " << failed << " failed\n";
}