cmake_minimum_required(VERSION 3.21)

# Conditional compiler setting
if(APPLE)
    # Looking for gcc installed by Homebrew
    find_program(GCC NAMES gcc-14 gcc-13 gcc-12 gcc-11)
    find_program(GXX NAMES g++-14 g++-13 g++-12 g++-11)
    if(GCC AND GXX)
        set(CMAKE_C_COMPILER ${GCC})
        set(CMAKE_CXX_COMPILER ${GXX})
    else()
        message(FATAL_ERROR "No suitable gcc/g++ compiler found. Please install gcc and g++.")
    endif()
elseif(UNIX)
    set(CMAKE_C_COMPILER gcc)
    set(CMAKE_CXX_COMPILER g++)
endif()

# Project name and version
project(APImageFilters VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src)

# Add the executable
file(GLOB_RECURSE HEADER_FILES ${CMAKE_SOURCE_DIR}/src/*.h)

add_executable(APImageFilters
    src/main.cpp
    src/DataContainer.cpp
    src/Image.cpp
    src/InputProcessor.cpp
    src/Pixel.cpp
    src/Slice.cpp
    src/Volume.cpp
    src/filter2D/BoxBlurFilter.cpp
    src/filter2D/ConvolutionFilter.cpp
    src/filter2D/Filter.cpp
    src/filter2D/GaussianBlurFilter.cpp
    src/filter2D/MedianBlurFilter.cpp
    src/filter2D/PrewittFilter.cpp
    src/filter2D/RobertsCrossFilter.cpp
    src/filter2D/SharpeningFilter.cpp
    src/filter2D/SimpleFilters.cpp
    src/filter2D/SobelFilter.cpp
    src/filter3D/Gaussian3DFilter.cpp
    src/filter3D/Median3DFilter.cpp
    src/filter3D/VolumeFilter.cpp
    src/projectionFunc/AvgIntensityProj.cpp 
    src/projectionFunc/MaxIntensityProj.cpp
    src/projectionFunc/MinIntensityProj.cpp
    src/projectionFunc/Projection.cpp
    ${HEADER_FILES}
)

# Enable testing
include(CTest)
enable_testing()

# Include the tests
include(${CMAKE_SOURCE_DIR}/cmdtests.cmake)
