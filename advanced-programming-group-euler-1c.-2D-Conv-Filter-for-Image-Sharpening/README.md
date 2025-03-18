# APImageFilters
Advanced Programming Image Filters Project

## CT Scans
Download CT Scan datasets here:
https://imperiallondon-my.sharepoint.com/:u:/g/personal/tmd02_ic_ac_uk/EafXMuNsbcNGnRpa8K62FjkBvIKvCswl1riz7hPDHpHdSQ

## Installation instructions

### MacOS or Linux

First, clone the repository:

```bash
git clone XXX
cd APImageFilters
```

Install cmake and a C++ compiler if you don't already have them. On MacOS, you can install them with Homebrew:

```bash
brew install cmake
brew install gcc
```

Then, create a build directory and run cmake:

```bash
cmake -S . -B build
```

Finally, compile the project:

```bash
cmake --build build
```

### Windows with MSVC

If you don't already have them installed on Windows (not WSL), you will need to install CMake and git. You can download CMake from [here](https://cmake.org/download/) and git from [here](https://git-scm.com/download/win).

First, in Windows Powershell, clone the repository:

```pwsh
git clone XXX
cd APImageFilters
```

Then, create a build directory and run cmake:

```pwsh
cmake -S . -B build -G "Visual Studio 17 2022"
```

Replace "Visual Studio 17 2022" with the appropriate version if you are using a different version of Visual Studio.

Open the generated solution file (.sln) in Visual Studio:

```pwsh
start build\APImageFilters.sln
```

Finally, compile the project in Visual Studio.

## Usage instructions

The program should be called with command line options. These are explained in the [Command line options](command_line_options.md) file.
