/** 
* @file InputProcessor.h
* @brief Declaration of the input processor - command line parser for image processing filters.
* @group [Euler]
* 
* Group members:
* - [Davide Baino] ([esemsc-db24])
* - [Qi Gao] ([esemsc-qg124])
* - [Daniel Kaupa] ([esemsc-dbk24])
* - [Leyao Liu] ([esemsc-ll1524])
* - [Jiayi Lu] ([esemsc-jl7324])
* - [Ananya Sinha] ([esemsc-as10524])
* - [Mingwei Yan] ([esemsc-my324])
*/

#ifndef INPUT_PROCESSOR_H
#define INPUT_PROCESSOR_H

#include "Image.h"
#include "filter2D/SimpleFilters.h"
#include "Volume.h"
#include "Slice.h"

#include "filter2D/BoxBlurFilter.h"
#include "filter2D/ConvolutionFilter.h"
#include "filter2D/GaussianBlurFilter.h"
#include "filter2D/MedianBlurFilter.h"
#include "filter2D/PrewittFilter.h"
#include "filter2D/RobertsCrossFilter.h"
#include "filter2D/SharpeningFilter.h"
#include "filter2D/SimpleFilters.h"
#include "filter2D/SobelFilter.h"

 #include "./projectionFunc/MaxIntensityProj.h"
 #include "./projectionFunc/MinIntensityProj.h"
 #include "./projectionFunc/AvgIntensityProj.h"
 #include "./filter3D/Gaussian3DFilter.h"
 #include "./filter3D/Median3DFilter.h"
 
 #include <iostream>
 #include <stdexcept>
 #include <ranges>    // For std::ranges::transform
 #include <algorithm>  // For std::transform
 #include <cctype>     // For std::tolower
 #include <regex>
 #include <memory>
 #include <filesystem>
 #include <cmath>
#include <string>
#include <unordered_map>
#include <vector>
#include <functional>


// Forward declarations
class Volume;

namespace fs = std::filesystem;

/**
 * @class InputProcessor
 * @brief Handles command-line argument parsing and executes image and volume processing operations.
 *
 * This class parses user-provided command-line arguments, validates input/output files,
 * and applies the selected processing operations in sequence. It can handle both 2D images
 * and 3D volumes.
 */
class InputProcessor {
private:
    std::string input_file;      ///< Path to the input image or data volume
    std::string output_file;     ///< Path to the output image (must be .png or .jpg)
    bool is_image;               ///< Flag to determine if input is an image (`true`) or data volume (`false`)
    
    // 3D volume-specific parameters
    int first_index;             ///< First slice index to load (optional)
    int last_index;              ///< Last slice index to load (optional)
    std::string file_extension;  ///< File extension for volume slices (default: png)
        
    std::vector<std::string> options;  ///< List of command-line options
    
    // Function maps for 2D image processing
    std::unordered_map<std::string, std::function<Image(const Image&, const std::vector<std::string>&)>> image_function_map;
    
    // Function maps for 3D volume processing
    std::unordered_map<std::string, std::function<std::unique_ptr<Volume>(const Volume&, const std::vector<std::string>&)>> volume_filter_map;
    std::unordered_map<std::string, std::function<Image(const Volume&, const std::vector<std::string>&)>> volume_projection_map;
    std::unordered_map<std::string, std::function<Image(const Volume&, const std::vector<std::string>&)>> volume_slice_map;

public:
    // *******************************************************************************************
    // -------------------------------------------------------------------------------------------
    // CONSTRUCTORS & DESTRUCTOR
    // -------------------------------------------------------------------------------------------
    // *******************************************************************************************
    /**
     * @brief Constructs an InputProcessor object and parses command-line arguments.
     * 
     * @param argc Number of command-line arguments.
     * @param argv Array of command-line argument strings.
     * @details This constructor validates input arguments and initialises the function map.
     *          The first argument must be `-i <input_image>` or `-d <data_volume>`, and the last 
     *          argument must be the output filename.
     * @throws std::invalid_argument If the number of arguments is less than 4 or input format is incorrect.
     */
    InputProcessor(int argc, char* argv[]);

    /**
     * @brief Destructor for the InputProcessor class
     */
    ~InputProcessor() {}

    // *******************************************************************************************
    // -------------------------------------------------------------------------------------------
    // INPUT PROCESSING
    // -------------------------------------------------------------------------------------------
    // *******************************************************************************************

    /**
     * @brief Normalises a command-line option by converting it to lowercase.
     * (e.g. `-G` -> `-g`, `--GREY` -> `--grey`)
     * 
     * @param option The command-line option to normalise.
     * @param volumeMode Flag to indicate if the command is for volume processing.
     * @return std::string The normalised option.
     */
    std::string normaliseOption(const std::string& option, bool volumeMode = false);

    /**
     * @brief Parses and validates the command-line arguments.
     * Determines whether the input is an image (`-i`) or a volume (`-d`), extracts the input/output filenames, and stores any additional filter options.
     * 
     * @throws std::invalid_argument If the argument count is incorrect.
     * @throws std::invalid_argument If the input file is missing or the format is invalid.
     */
    void parseArguments(int argc, char* argv[]);

    /**
     * @brief Converts a string to lowercase.
     * 
     * @param str The string to convert to lowercase.
     * @return std::string The lowercase string.
     */
    std::string toLowercase(const std::string& str);

    // *******************************************************************************************
    // -------------------------------------------------------------------------------------------
    // INITIALISING MAP OF FUNCTIONS AND COMMANDS
    // -------------------------------------------------------------------------------------------
    // *******************************************************************************************

    /**
     * @brief Initialises the function map, linking CLI options to filter functions.
     * 
     * This method associates each command-line option (e.g., `--greyscale`, `-g`) with 
     * the corresponding image processing function. These functions will be executed 
     * in sequence when `executeFilters()` is called.
     */
    void initialiseFunctionMap();

    /**
     * @brief Initializes the 3D volume filter and processing function maps.
     */
    void initialiseVolumeFunctionMap();

    // *******************************************************************************************
    // -------------------------------------------------------------------------------------------
    // EXECUTING FILTERS
    // -------------------------------------------------------------------------------------------
    // *******************************************************************************************
    /**
     * @brief Applies all selected filters to the input image or volume.
     * 
     * This method sequentially applies the filters specified via command-line arguments
     * and saves the final processed image to the output file.
     */
    void executeFilters();

    /**
     * @brief Process the 2D image with the specified filters.
     * 
     * @return bool True if processing was successful, false otherwise.
     */
    bool processImage();

    /**
     * @brief Process the 3D volume with the specified operations.
     * 
     * @return bool True if processing was successful, false otherwise.
     */
    bool processVolume();

    // *******************************************************************************************
    // -------------------------------------------------------------------------------------------
    // VOLUME LOADING & HELPER METHODS
    // -------------------------------------------------------------------------------------------
    // *******************************************************************************************

     /**
     * @brief Loads a 3D volume from the specified input path.
     * 
     * @return std::unique_ptr<Volume> Pointer to the loaded volume.
     * @throws std::runtime_error If the directory or files cannot be loaded.
     */
    std::unique_ptr<Volume> loadVolume();
    
    /**
     * @brief Compares strings for natural sort order (e.g., "10" comes after "2").
     * 
     * @param a First string to compare.
     * @param b Second string to compare.
     * @return bool True if a should come before b in natural sort.
     */
    bool naturalCompare(const std::string& a, const std::string& b);

    /**
     * @brief Displays help information for volume processing.
     */
    void showVolumeHelp();

    // *******************************************************************************************
    // -------------------------------------------------------------------------------------------
    // GETTERS & SETTERS
    // -------------------------------------------------------------------------------------------
    // *******************************************************************************************
    
    /**
     * @brief Gets all image files in a directory with the specified extension.
     * 
     * @param directoryPath Path to the directory containing images.
     * @param extension File extension to filter by (without dot, e.g., "png").
     * @return std::vector<std::string> List of full paths to matching files.
     */
    std::vector<std::string> getImageFilesInDirectory(const std::string& directoryPath, const std::string& extension);

    /**
     * @brief Gets the output filename.
     * @return std::string The output file.
     */
    std::string getOutputFile() const { return output_file; }
};

#endif  // INPUT_PROCESSOR_H