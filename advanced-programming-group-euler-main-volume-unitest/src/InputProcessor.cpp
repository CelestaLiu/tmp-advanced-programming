/**
 * @file InputProcessor.cpp
 * @brief Implementation of the input processor - command line parser for image processing filters.
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
 
 #include "InputProcessor.h"

 namespace fs = std::filesystem;


 // *******************************************************************************************  
 // -------------------------------------------------------------------------------------------
 // CONSTRUCTORS
 // -------------------------------------------------------------------------------------------
 // *******************************************************************************************
 
// Constructor
InputProcessor::InputProcessor(int argc, char* argv[]) 
    : first_index(-1), last_index(-1), file_extension("png") {
    parseArguments(argc, argv);
    initialiseFunctionMap();
    initialiseVolumeFunctionMap();
}
  
 // *******************************************************************************************
 // -------------------------------------------------------------------------------------------
 // INPUT PROCESSING
 // -------------------------------------------------------------------------------------------
 // *******************************************************************************************
 
// Parse command-line arguments
void InputProcessor::parseArguments(int argc, char* argv[]) {
    if (argc < 4) {
        throw std::invalid_argument("Usage: ./APImageFilters -i <input> [options] <output> or ./APImageFilters -d <data_volume> [options] <output>");
    }

    std::string first_arg = normaliseOption(argv[1]);
    if (first_arg == "-i") {
        is_image = true;
        input_file = argv[2];
    } else if (first_arg == "-d") {
        is_image = false;
        input_file = argv[2];
    } else {
        throw std::invalid_argument("First argument must be -i <input_image> or -d <data_volume>");
    }

    output_file = argv[argc - 1];

    // Parse options, handling volume-specific options like --first, --last, --extension
    for (int i = 3; i < argc - 1; ++i) {
        std::string option = normaliseOption(argv[i], !is_image);  // Pass mode flag - true for volume, false for image
        
        // Handle volume-specific options
        if (!is_image && (option == "--first" || option == "-f")) {
            if (i + 1 < argc - 1) {
                first_index = std::stoi(argv[++i]);
            }
        } else if (!is_image && (option == "--last" || option == "-l")) {
            if (i + 1 < argc - 1) {
                last_index = std::stoi(argv[++i]);
            }
        } else if (!is_image && (option == "--extension" || option == "-x")) {
            if (i + 1 < argc - 1) {
                file_extension = argv[++i];
            }
        } else {
            // Store regular filter/processing options
            options.push_back(option);
            
            // Collect non-flag parameters
            while (i + 1 < argc - 1 && argv[i + 1][0] != '-') {
                options.push_back(argv[++i]);
            }
        }
    }
}
 // *******************************************************************************************
  // -------------------------------------------------------------------------------------------
  // INPUT PROCESSING
  // -------------------------------------------------------------------------------------------
  // *******************************************************************************************
  
  // Normalise a command-line inputs with context awareness
  std::string InputProcessor::normaliseOption(const std::string& option, bool volumeMode) {
     // Convert to lowercase
     std::string normalised = option;
     std::transform(normalised.begin(), normalised.end(), normalised.begin(),
     [](unsigned char c) { return std::tolower(c); });
 
     // Common aliases for both modes
     static const std::unordered_map<std::string, std::string> common_aliases = {
         {"-i", "-i"}, {"-I", "-i"}, {"--image", "-i"},         // Image input
         {"-d", "-d"}, {"--data", "-d"},                        // Volume input
         {"-r", "--blur"}, {"--gaussianblur", "--blur"},        // Both modes use blur
         {"-h", "--help"}, {"--h", "--help"}                    // Help flag
     };
 
     // 2D image-specific aliases
     static const std::unordered_map<std::string, std::string> image_aliases = {
         {"-g", "--greyscale"}, {"--gray", "--greyscale"},
         {"-b", "--brightness"}, {"--bright", "--brightness"},
         {"-h", "--histogram"}, {"--hist", "--histogram"},
         {"-e", "--edge"}, {"--edgedetect", "--edge"},
         {"-p", "--sharpen"}, {"--sharp", "--sharpen"},
         {"-n", "--saltpepper"}, {"--noise", "--saltpepper"},
         {"-t", "--threshold"}, {"--thresh", "--threshold"}
     };
 
     // 3D volume-specific aliases
     static const std::unordered_map<std::string, std::string> volume_aliases = {
         {"-s", "--slice"}, {"--cut", "--slice"},               // Slice
         {"-p", "--projection"}, {"--proj", "--projection"},    // Projection
         {"-f", "--first"}, {"--start", "--first"},             // First index
         {"-l", "--last"}, {"--end", "--last"},                 // Last index
         {"-x", "--extension"}, {"--ext", "--extension"}        // File extension
     };
 
     // Check common aliases first
     if (common_aliases.find(normalised) != common_aliases.end()) {
         return common_aliases.at(normalised);
     }
 
     // Then check mode-specific aliases
     const auto& mode_map = volumeMode ? volume_aliases : image_aliases;
     if (mode_map.find(normalised) != mode_map.end()) {
         return mode_map.at(normalised);
     }
 
     return normalised; // Return the transformed option
 }
 
 // Helper function to convert a string to lowercase
 std::string InputProcessor::toLowercase(const std::string& str) {
     std::string result = str;
     std::transform(result.begin(), result.end(), result.begin(), ::tolower);
     return result;
 }
 
 // *******************************************************************************************
 // -------------------------------------------------------------------------------------------
 // INITIALISING MAP OF FUNCTIONS AND COMMANDS
 // -------------------------------------------------------------------------------------------
 // *******************************************************************************************
 
 // Initialise the function map, linking CLI options to filter functions
 void InputProcessor::initialiseFunctionMap() {
     // -----------------------
     // 2D Image Processing
     // -----------------------
     image_function_map["--greyscale"] = [](const Image& img, const std::vector<std::string>&) { 
         return SimpleFilters("Greyscale").apply(img); 
     };
     image_function_map["-g"] = image_function_map["--greyscale"];
 
     image_function_map["--brightness"] = [](const Image& img, const std::vector<std::string>& args) { 
         int brightness = (!args.empty()) ? std::stoi(args[0]) : 50; // Default: 50
         return SimpleFilters("Brightness").applyBrightness(img, brightness);
     };
     image_function_map["-b"] = image_function_map["--brightness"];
 
     image_function_map["--histogram"] = [](const Image& img, const std::vector<std::string>& args) { 
         std::string type = (!args.empty()) ? args[0] : "HSV"; // Default to HSV
         return SimpleFilters("HistogramEqualization").apply(img);
     };
     image_function_map["-h"] = image_function_map["--histogram"];
 
     image_function_map["--threshold"] = [](const Image& img, const std::vector<std::string>& args) { 
         int threshold = (!args.empty()) ? std::stoi(args[0]) : 128;
         return SimpleFilters("Thresholding").applyThresholding(img, threshold);
     };
     image_function_map["-t"] = image_function_map["--threshold"];
 
     image_function_map["--saltpepper"] = [](const Image& img, const std::vector<std::string>& args) { 
         float noise = (!args.empty()) ? std::stof(args[0]) : 5.0f; 
         return SimpleFilters("SaltAndPepperNoise").applySaltAndPepperNoise(img, noise);
     };
     image_function_map["-n"] = image_function_map["--saltpepper"];
 
     // -----------------------
     // Blurring Filters
     // -----------------------
     image_function_map["--blur"] = [](const Image& img, const std::vector<std::string>& args) {
         if (args.size() < 2) {
             throw std::invalid_argument("Blur filter requires <type> <size> [<stdev>].");
         }
         std::string type = args[0];
         int size = std::stoi(args[1]);
         float stdev = (args.size() > 2) ? std::stof(args[2]) : 2.0f;
 
         if (type == "Gaussian") {
            return GaussianBlurFilter(size, stdev).apply(img);
         } else if (type == "Box") {
            return BoxBlurFilter(size).apply(img);
         } else if (type == "Median") {
            return MedianBlurFilter(size).apply(img);
         } else {
             throw std::invalid_argument("Invalid blur type.");
         }
     };
     image_function_map["-r"] = image_function_map["--blur"];
 
    // -----------------------
    // Sharpening Filter
    // -----------------------
     image_function_map["--sharpen"] = [](const Image& img, const std::vector<std::string>&) {
        return SharpeningFilter().apply(img);
    };
        image_function_map["-p"] = image_function_map["--sharpen"];

     // -----------------------
     // Edge Detection
     // -----------------------
     image_function_map["--edge"] = [](const Image& img, const std::vector<std::string>& args) {
         if (args.size() < 1) {
             throw std::invalid_argument("Edge detection requires <type>.");
         }
         std::string type = args[0];
 
         if (type == "Sobel") {
            return SobelFilter().apply(img);
         } else if (type == "Prewitt") {
            return PrewittFilter().apply(img);
         } else if (type == "RobertsCross") {
            return RobertsCrossFilter().apply(img);
         } else {
             throw std::invalid_argument("Invalid edge detection type.");
         }
     };
     image_function_map["-e"] = image_function_map["--edge"];
 }
 
 // Initialise the volume function map, linking CLI options to volume processing functions
 void InputProcessor::initialiseVolumeFunctionMap() {
     // -----------------------
     // Volume Filters
     // -----------------------
     volume_filter_map["--blur"] = [this](const Volume& vol, const std::vector<std::string>& args) {
         if (args.size() < 2) {
             throw std::invalid_argument("Blur filter requires <type> <size> [<stdev>].");
         }
         
         std::string type = toLowercase(args[0]);
         int kernelSize = std::stoi(args[1]);
         
         if (type == "gaussian") {
             float sigma = (args.size() > 2) ? std::stof(args[2]) : 2.0f;
             return vol.applyGaussianFilter(kernelSize, sigma);
         } else if (type == "median") {
             return vol.applyMedianFilter(kernelSize);
         } else {
             throw std::invalid_argument("Invalid 3D blur type. Use 'Gaussian' or 'Median'.");
         }
     };
     volume_filter_map["-r"] = volume_filter_map["--blur"];
 
     // -----------------------
     // Volume Projections
     // -----------------------
     volume_projection_map["--projection"] = [this](const Volume& vol, const std::vector<std::string>& args) {
         if (args.empty()) {
             throw std::invalid_argument("Projection requires <type>.");
         }
         
         std::string type = toLowercase(args[0]);
         
         if (type == "mip") {
             MaxIntensityProj mip;
             return vol.applyProjection(mip);
         } else if (type == "minip") {
             MinIntensityProj minip;
             return vol.applyProjection(minip);
         } else if (type == "meanaip") {
             AvgIntensityProj aip(0, -1, false); // Use mean
             return vol.applyProjection(aip);
         } else if (type == "medianaip") {
             AvgIntensityProj aip(0, -1, true); // Use median
             return vol.applyProjection(aip);
         } else {
             throw std::invalid_argument("Invalid projection type. Use 'MIP', 'MinIP', 'meanAIP', or 'medianAIP'.");
         }
     };
     volume_projection_map["-p"] = volume_projection_map["--projection"];
 
     // -----------------------
     // Volume Slices
     // -----------------------
     volume_slice_map["--slice"] = [this](const Volume& vol, const std::vector<std::string>& args) {
         if (args.size() < 2) {
             throw std::invalid_argument("Slice requires <plane> <constant>.");
         }
         
         std::string plane = toLowercase(args[0]);
         int constant = std::stoi(args[1]);
         
         Slice slice;
         if (plane == "xy") {
             slice = Slice(SlicePlane::XY, constant);
         } else if (plane == "xz") {
             slice = Slice(SlicePlane::XZ, constant);
         } else if (plane == "yz") {
             slice = Slice(SlicePlane::YZ, constant);
         } else {
             throw std::invalid_argument("Invalid slice plane. Use 'XY', 'XZ', or 'YZ'.");
         }
         
         return vol.extractSlice(slice);
     };
     volume_slice_map["-s"] = volume_slice_map["--slice"];
 }
 
 
 // *******************************************************************************************
 // -------------------------------------------------------------------------------------------
 // EXECUTING FILTERS
 // -------------------------------------------------------------------------------------------
 // *******************************************************************************************
 
 // Apply all selected filters to the input image or volume
 void InputProcessor::executeFilters() {
     try {
         if (is_image) {
             // Process 2D image
             if (!processImage()) {
                 throw std::runtime_error("Failed to process image");
             }
         } else {
             // Process 3D volume
             if (!processVolume()) {
                 throw std::runtime_error("Failed to process volume");
             }
         }
     } catch (const std::exception& e) {
         std::cerr << "Error: " << e.what() << std::endl;
         throw;
     }
 }
 
  // Process the 2D image with the specified filters (includes error handling)
  bool InputProcessor::processImage() {
      try {
          Image img(input_file);
  
          for (size_t i = 0; i < options.size(); i++) {
              std::string option = normaliseOption(options[i]);
              std::vector<std::string> params;
  
              while (i + 1 < options.size() && options[i + 1][0] != '-') {
                  params.push_back(options[++i]);  // Collect non-flag parameters
              }
  
              if (image_function_map.find(option) != image_function_map.end()) {
                  try {
                      img = image_function_map[option](img, params);
                  } catch (const std::exception& e) {
                      std::cerr << "Error applying filter '" << option << "': " << e.what() << std::endl;
                  }
              } else {
                  std::cerr << "Unknown image filter: " << option << std::endl;
              }
          }
  
          if (!img.saveToFile(output_file)) {
              throw std::runtime_error("Failed to save output image: " + output_file);
          }
  
          return true;
      } catch (const std::exception& e) {
          std::cerr << "Error processing image: " << e.what() << std::endl;
          return false;
      }
  }
  // Process the 3D volume with the specified operations (includes error handling)
  bool InputProcessor::processVolume() {
      try {
          // Load the volume
          auto vol = loadVolume();
          std::cout << "Volume loaded successfully. Dimensions: " 
                    << std::get<0>(vol->getDimensions3D()) << "x"
                    << std::get<1>(vol->getDimensions3D()) << "x"
                    << std::get<2>(vol->getDimensions3D()) << std::endl;
          
          // Track whether we need to output a slice or projection
          bool hasSliceOrProjection = false;
          std::unique_ptr<Image> outputImage = nullptr;
          
          // Process options in the order they were provided
          for (size_t i = 0; i < options.size(); ) {
              std::string option = normaliseOption(options[i++], true);  // Pass true for volume mode
              std::vector<std::string> params;
              
              // Collect non-flag parameters
              while (i < options.size() && options[i][0] != '-') {
                  params.push_back(options[i++]);
              }
              
              // Apply volume filters
              if (volume_filter_map.find(option) != volume_filter_map.end()) {
                  std::cout << "Applying filter: " << option;
                  for (const auto& param : params) {
                      std::cout << " " << param;
                  }
                  std::cout << std::endl;
                  
                  vol = volume_filter_map[option](*vol, params);
                  std::cout << "Filter applied successfully." << std::endl;
              } 
              // Apply projections
              else if (volume_projection_map.find(option) != volume_projection_map.end()) {
                  std::cout << "Creating projection: " << option;
                  for (const auto& param : params) {
                      std::cout << " " << param;
                  }
                  std::cout << std::endl;
                  
                  Image result = volume_projection_map[option](*vol, params);
                  outputImage = std::make_unique<Image>(result);
                  hasSliceOrProjection = true;
                  std::cout << "Projection created successfully." << std::endl;
              } 
              // Apply slices
              else if (volume_slice_map.find(option) != volume_slice_map.end()) {
                  std::cout << "Extracting slice: " << option;
                  for (const auto& param : params) {
                      std::cout << " " << param;
                  }
                  std::cout << std::endl;
                  
                  Image result = volume_slice_map[option](*vol, params);
                  outputImage = std::make_unique<Image>(result);
                  hasSliceOrProjection = true;
                  std::cout << "Slice extracted successfully." << std::endl;
              } 
              else if (option == "--help" || option == "-h") {
                  showVolumeHelp();
                  return true;
              }
              else if (option[0] == '-') {
                  std::cerr << "Warning: Unknown option: " << option << std::endl;
              }
          }
          
          // If no slice or projection was applied, use middle slice as default
          if (!hasSliceOrProjection) {
              std::cout << "No slice or projection specified. Using middle XY slice as default." << std::endl;
              Slice defaultSlice(SlicePlane::XY, vol->getDepth() / 2 + 1); // +1 because Slice constructor expects 1-based index
              Image result = vol->extractSlice(defaultSlice);
              outputImage = std::make_unique<Image>(result);
          }
          
          // Save the output image
          std::cout << "Saving output to " << output_file << "..." << std::endl;
          if (!outputImage->saveToFile(output_file)) {
              throw std::runtime_error("Failed to save output image: " + output_file);
          }
          std::cout << "Output saved successfully." << std::endl;
          
          return true;
      } catch (const std::exception& e) {
          std::cerr << "Error processing volume: " << e.what() << std::endl;
          return false;
      }
  }
  
  // *******************************************************************************************
  // -------------------------------------------------------------------------------------------
  // VOLUME LOADING & HELPER METHODS
  // -------------------------------------------------------------------------------------------
  // *******************************************************************************************
  
  // Helper function for natural sort of filenames
  bool InputProcessor::naturalCompare(const std::string& a, const std::string& b) {
      auto extractNumbersAndText = [](const std::string& str) {
          std::vector<std::pair<int, std::string>> result;
          std::string text;
          std::string digits;
          
          for (char c : str) {
              if (std::isdigit(c)) {
                  digits += c;
              } else {
                  if (!digits.empty()) {
                      result.push_back({std::stoi(digits), text});
                      text.clear();
                      digits.clear();
                  }
                  text += c;
              }
          }
          
          if (!digits.empty()) {
              result.push_back({std::stoi(digits), text});
          } else if (!text.empty()) {
              result.push_back({0, text});
          }
          
          return result;
      };
      
      auto aParts = extractNumbersAndText(a);
      auto bParts = extractNumbersAndText(b);
      
      for (size_t i = 0; i < std::min(aParts.size(), bParts.size()); ++i) {
          if (aParts[i].second != bParts[i].second) {
              return aParts[i].second < bParts[i].second;
          }
          if (aParts[i].first != bParts[i].first) {
              return aParts[i].first < bParts[i].first;
          }
      }
      
      return aParts.size() < bParts.size();
  }
  
  std::vector<std::string> InputProcessor::getImageFilesInDirectory(const std::string& directoryPath, const std::string& extension) {
      std::vector<std::string> filenames;
      try {
          // Check if directory exists
          if (!fs::exists(directoryPath) || !fs::is_directory(directoryPath)) {
              std::cerr << "Error: " << directoryPath << " is not a valid directory." << std::endl;
              return filenames;
          }
          
          // Prepare extension string for comparison
          std::string dotExtension = "." + extension;
          std::transform(dotExtension.begin(), dotExtension.end(), dotExtension.begin(), ::tolower);
          
          // Collect all matching files
          for (const auto& entry : fs::directory_iterator(directoryPath)) {
              if (entry.is_regular_file()) {
                  std::string ext = entry.path().extension().string();
                  std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
                  if (ext == dotExtension) {
                      filenames.push_back(entry.path().string());
                  }
              }
          }
          
          // Sort filenames using natural sort to ensure correct sequential order
          std::sort(filenames.begin(), filenames.end(), 
                    [this](const std::string& a, const std::string& b) {
                        return naturalCompare(a, b);
                    });
      }
      catch (const fs::filesystem_error& ex) {
          std::cerr << "Filesystem error: " << ex.what() << std::endl;
      }
      catch (const std::exception& ex) {
          std::cerr << "Error: " << ex.what() << std::endl;
      }
      
      return filenames;
  }
  
  std::unique_ptr<Volume> InputProcessor::loadVolume() {
      try {
          std::cout << "Loading volume from " << input_file << "..." << std::endl;
          
          // Check if input path is a directory
          if (fs::is_directory(input_file)) {
              // Get all image files in the directory
              std::vector<std::string> allFiles = getImageFilesInDirectory(input_file, file_extension);
              
              if (allFiles.empty()) {
                  throw std::runtime_error("No matching files found in directory: " + input_file);
              }
              
              std::cout << "Found " << allFiles.size() << " image files in directory." << std::endl;
              
              // If first_index and last_index are specified, select the subset
              if (first_index >= 0 && last_index >= 0) {
                  // Convert to zero-based indices for vector access
                  int zeroBasedFirst = first_index - 1;
                  int zeroBasedLast = last_index - 1;
                  
                  // Validate indices
                  if (zeroBasedFirst < 0 || zeroBasedLast >= static_cast<int>(allFiles.size()) || zeroBasedFirst > zeroBasedLast) {
                      std::cerr << "Warning: Specified index range (" << first_index << " to " << last_index 
                                << ") is out of bounds for found files (1 to " << allFiles.size() << ")." << std::endl;
                      std::cerr << "Using available range instead." << std::endl;
                      
                      // Adjust to available range
                      zeroBasedFirst = std::max(0, std::min(zeroBasedFirst, static_cast<int>(allFiles.size()) - 1));
                      zeroBasedLast = std::max(zeroBasedFirst, std::min(zeroBasedLast, static_cast<int>(allFiles.size()) - 1));
                  }
                  
                  // Extract the subset of files
                  std::vector<std::string> selectedFiles(allFiles.begin() + zeroBasedFirst, allFiles.begin() + zeroBasedLast + 1);
                  
                  std::cout << "Selected " << selectedFiles.size() << " files from index " 
                            << (zeroBasedFirst + 1) << " to " << (zeroBasedLast + 1) << "." << std::endl;
                  
                  return std::make_unique<Volume>(selectedFiles);
              } else {
                  // Use all files
                  return std::make_unique<Volume>(allFiles);
              }
          } else {
              // Input path is not a directory
              std::string directoryPath = fs::path(input_file).parent_path().string();
              
              if (directoryPath.empty()) {
                  directoryPath = "."; // Use current directory if no parent path
              }
              
              std::vector<std::string> allFiles = getImageFilesInDirectory(directoryPath, file_extension);
              
              if (allFiles.empty()) {
                  throw std::runtime_error("No matching files found in directory: " + directoryPath);
              }
              
              // If first_index and last_index are specified, select the subset
              if (first_index >= 0 && last_index >= 0) {
                  // Convert to zero-based indices for vector access
                  int zeroBasedFirst = first_index - 1;
                  int zeroBasedLast = last_index - 1;
                  
                  // Validate indices
                  if (zeroBasedFirst < 0 || zeroBasedLast >= static_cast<int>(allFiles.size()) || zeroBasedFirst > zeroBasedLast) {
                      std::cerr << "Warning: Specified index range (" << first_index << " to " << last_index 
                                << ") is out of bounds for found files (1 to " << allFiles.size() << ")." << std::endl;
                      std::cerr << "Using available range instead." << std::endl;
                      
                      // Adjust to available range
                      zeroBasedFirst = std::max(0, std::min(zeroBasedFirst, static_cast<int>(allFiles.size()) - 1));
                      zeroBasedLast = std::max(zeroBasedFirst, std::min(zeroBasedLast, static_cast<int>(allFiles.size()) - 1));
                  }
                  
                  // Extract the subset of files
                  std::vector<std::string> selectedFiles(allFiles.begin() + zeroBasedFirst, allFiles.begin() + zeroBasedLast + 1);
                  
                  std::cout << "Selected " << selectedFiles.size() << " files from index " 
                            << (zeroBasedFirst + 1) << " to " << (zeroBasedLast + 1) << "." << std::endl;
                  
                  return std::make_unique<Volume>(selectedFiles);
              } else {
                  // Use all files
                  return std::make_unique<Volume>(allFiles);
              }
          }
      } catch (const std::exception& e) {
          std::cerr << "Error loading volume: " << e.what() << std::endl;
          throw;
      }
  }
  
  void InputProcessor::showVolumeHelp() {
      std::cout << "Volume Processing Tool - Usage:" << std::endl;
      std::cout << "  ./APImageFilters -d <input_path> [options] <output_image>" << std::endl;
      std::cout << std::endl;
      std::cout << "Options:" << std::endl;
      std::cout << "  Volume Reading:" << std::endl;
      std::cout << "    --first <index>, -f <index>          First slice index to load (optional)" << std::endl;
      std::cout << "    --last <index>, -l <index>           Last slice index to load (optional)" << std::endl;
      std::cout << "    --extension <ext>, -x <ext>          File extension (default: png)" << std::endl;
      std::cout << std::endl;
      std::cout << "  Volume Filters:" << std::endl;
      std::cout << "    --blur <type> <size> [<stdev>], -r <type> <size> [<stdev>]" << std::endl;
      std::cout << "                                         Apply 3D blur filter" << std::endl;
      std::cout << "                                         Types: Gaussian, Median" << std::endl;
      std::cout << "                                         <stdev> only required for Gaussian" << std::endl;
      std::cout << std::endl;
      std::cout << "  Volume Processing:" << std::endl;
      std::cout << "    --slice <plane> <constant>, -s <plane> <constant>" << std::endl;
      std::cout << "                                         Extract a slice from the volume" << std::endl;
      std::cout << "                                         Planes: XY, XZ, YZ" << std::endl;
      std::cout << "    --projection <type>, -p <type>       Create a projection from the volume" << std::endl;
      std::cout << "                                         Types: MIP, MinIP, meanAIP, medianAIP" << std::endl;
      std::cout << std::endl;
      std::cout << "Examples:" << std::endl;
      std::cout << "  ./APImageFilters -d ./Scans/TestVolume -f 1 -l 50 --blur Gaussian 3 2.0 -p MIP output.png" << std::endl;
      std::cout << "  ./APImageFilters -d ./Scans/TestVolume -s XZ 16 output.png" << std::endl;
      std::cout << "  ./APImageFilters -d ./Scans/TestVolume -r Median 3 -p MinIP output.png" << std::endl;
  }