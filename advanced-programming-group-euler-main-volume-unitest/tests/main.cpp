#include <iostream>
#include "TestCounters.h"  // Include the header

// Forward declarations of test functions.

void runGaussian3DFilterTests();
void runMedian3DFilterTests();
void runVolumeTests();
void runProjectionTests();
void runMaxIntensityProjTests();
void runMinIntensityProjTests();
void runAvgIntensityProjTests();
void runSliceTests();

int main() {
    std::cout << "Running all unit tests..." << std::endl;
    
    // runGaussian3DFilterTests();
    runMedian3DFilterTests();
    runVolumeTests();
    runProjectionTests();
    runMaxIntensityProjTests();
    runMinIntensityProjTests();
    runAvgIntensityProjTests();
    runSliceTests();
    
    std::cout << "\nAll tests completed. "
              << passed << " passed, " << failed << " failed." << std::endl;

    return (failed > 0) ? EXIT_FAILURE : EXIT_SUCCESS;
}
