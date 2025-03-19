#include "InputProcessor.h"

#include <iostream>
#include <cassert>
#include <string>
#include <filesystem>
#include <stdexcept>

int main(int argc, char* argv[]) {
    try {
        std::cout << "Welcome to APImageFilters!" << std::endl;
        InputProcessor processor(argc, argv);
        processor.executeFilters();
        std::cout << "Processing complete! Output saved to: " << processor.getOutputFile() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;  // Exit with error code
    }
    return 0;
}
// TESTINGS
// ./build/APImageFilters -i Images/gracehopper.png --greyscale build/output/gracehopper_greyscale.png
// ./build/APImageFilters -i Images/gracehopper.png --brightness 100 build/output/gracehopper_brightness_100.png
// ./build/APImageFilters -i Images/gracehopper.png --brightness -100 build/output/gracehopper_brightness_-100.png
// ./build/APImageFilters -i Images/gracehopper.png --histogram build/output/gracehopper_histogram_hsv.png
// ./build/APImageFilters -i Images/gracehopper.png --threshold 128 build/output/gracehopper_threshold_128.png
// ./build/APImageFilters -i Images/gracehopper.png --saltpepper 10 build/output/gracehopper_saltpepper_10.png
// ./build/APImageFilters -i Images/gracehopper.png --blur Gaussian 5 1.5 build/output/gracehopper_gaussian_5x5.png
// ./build/APImageFilters -i Images/gracehopper.png --blur Box 3 build/output/gracehopper_box_3x3.png
// ./build/APImageFilters -i Images/gracehopper.png --blur Median 5 build/output/gracehopper_median_5x5.png
// ./build/APImageFilters -i Images/gracehopper.png --sharpen build/output/gracehopper_sharpened.png
// ./build/APImageFilters -i Images/gracehopper.png --edge Sobel build/output/gracehopper_sobel.png
// ./build/APImageFilters -i Images/gracehopper.png --edge Prewitt build/output/gracehopper_prewitt.png
// ./build/APImageFilters -i Images/gracehopper.png --edge RobertsCross build/output/gracehopper_roberts_cross.png

// ./build/APImageFilters -d Scans/TestVolume/vol000.png --blur Gaussian 5 1.5 build/output/TestVolume_gaussian_5x5x5.png
// ./build/APImageFilters -d Scans/TestVolume/vol000.png --blur Gaussian 3 build/output/TestVolume_gaussian_3x3x3.png
// ./build/APImageFilters -d Scans/TestVolume/vol000.png --blur Median 5 build/output/TestVolume_median_5x5x5.png
// ./build/APImageFilters -d Scans/TestVolume/vol000.png --blur Median 3 build/output/TestVolume_median_3x3x3.png
// ./build/APImageFilters -d Scans/TestVolume/vol000.png --projection MIP build/output/TestVolume_mip.png
// ./build/APImageFilters -d Scans/TestVolume/vol000.png --projection MinIP build/output/TestVolume_minip.png
// ./build/APImageFilters -d Scans/TestVolume/vol000.png --projection meanAIP build/output/TestVolume_meanaip.png
// ./build/APImageFilters -d Scans/TestVolume/vol000.png --projection medianAIP build/output/TestVolume_medianaip.png
// ./build/APImageFilters -d Scans/TestVolume/vol000.png --slice XY 10 build/output/TestVolume_slice_xy_10.png
// ./build/APImageFilters -d Scans/TestVolume/vol000.png --slice XZ 20 build/output/TestVolume_slice_xz_20.png
// ./build/APImageFilters -d Scans/TestVolume/vol000.png --slice YZ 30 build/output/TestVolume_slice_yz_30.png