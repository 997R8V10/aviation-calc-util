# Aviation Calc Util

A library to assist with complex aviation-related calculations. Primarily used for the VATSIM ATC Training Simulator
Project. The library is written in C++ 20 standard and requires a compiler that is capable of building to this standard.

## Overview

The library contains utilities to handle geographical calculations, magnetic calculations, and some math utilties.

## Dependencies

The project depends on the following libraries and tools:

### Windows

- **C & C++ 20 Compiler**
    - Visual Studio with C++ development enabled includes the compilers.
- **[CMake 3.14+](https://cmake.org/download/)**
    - C++ Build Automation Tool
- **[Conan 1.39+](https://conan.io/downloads.html)**
    - C++ Dependency Manager
- **[Python 3.0+](https://www.python.org/downloads/)**
    - Required for ecCodes build.
- **[Boost 1.76.0+](https://www.boost.org/)**
    - No Download Required
    - Fetched through Conan
- **[ecCodes 2.22.1](https://confluence.ecmwf.int/display/ECC)**
    - No Download or manual build required.
    - See [below](#eccodes)

### Linux/MacOS

- **C & C++ 20 Compiler**
    - Should come standard with a C & C++ compiler.
- **[CMake 3.14+](https://cmake.org/download/)**
    - C++ Build Automation Tool
    - Installation:
      - **Ubuntu**: `sudo apt install cmake`
      - **MacOS**: `brew install cmake`
- **[Conan 1.39+](https://conan.io/downloads.html)**
    - C++ Dependency Manager
    - Installation:
      - **PIP**: `sudo pip3 install conan`
      - **MacOS**: `brew install conan`
- **[Python 3.0+](https://www.python.org/downloads/)**
    - Required for ecCodes build.
    - Installation:
      - **Ubuntu**: `sudo apt install python3 python3-pip`
- **[Boost 1.76.0+](https://www.boost.org/)**
    - No Download Required
    - Fetched through Conan
- **[ecCodes 2.22.1](https://confluence.ecmwf.int/display/ECC)**
    - No Download or manual build required.
    - See [below](#eccodes)
- **Curl**
    - Both the library and dev packages must be installed
    - Installation:
        - **Ubuntu**: `sudo apt install libcurl4-openssl-dev`
        - **MacOS**: XCode developer tools will ship with libcurl dev.

### ecCodes

As ecCodes has no Conan or NuGet package, it must be built manually. Fortunately, a `conanfile.py` has been included in
the `eccodes` directory.

To install the package data, simply run `conan export` in the `eccodes` directory.

_**Note:** Building the NuGet package will automatically export ecCodes_

## Building

Below are the steps to build the code manually.

_**Note:** Building the NuGet package will automatically build for all configurations_

1. Create a `build` folder.
2. Install dependencies via Conan.
    ```
    conan install <source-folder> -if <build-folder> --build=missing
    ```
    1. Pass in any platform/configuration settings here
    2. For example: `-s build_type=Debug -s arch=x86`
3. Generate CMake files.
   ```
   cmake -S <source-folder> -B <build-folder>
    ```
    1. Pass in the same platform/configuration settings from before
    2. For example: `-DCMAKE_BUILD_TYPE=Debug -A Win32`
4. Build the project.
    ```
    cmake --build <build-folder> --target aviationcalc
    ```

### Conan Package

To generate the Conan package, simply run `conan export` in the source folder.

_**Note:** ecCodes **MUST** be built prior to building the main package_

### NuGet Package
To create a NuGet package, simply follow the instructions depending on your operating system below.

Then add your local NuGet feed to Visual Studio.

The project will be available in Visual Studio as a NuGet package under the name `PShivaraman.AviationCalcUtil`

#### Windows
Simply run `create-nuget-package.bat` to generate a NuGet package. The default feed is set
to `%USERPROFILE%\.nuget\feed`.

Overriding default feed can be accomplished by passing your desired feed in as an argument: \
`create-nuget-package.bat S:\data\nugetfeed`

#### Unix (Mono)
[Mono](https://www.mono-project.com/download/stable/) is required to build nuget packages on Unix systems.

**MacOS**: Install wget through brew.

Simply run `create-nuget-package-dotnet.sh` to generate a NuGet package. The default feed is set
to `$HOME/.nuget/feed`.

Overriding default feed can be accomplished by passing your desired feed in as an argument: \
`create-nuget-package-dotnet.sh ~/data/nugetfeed`