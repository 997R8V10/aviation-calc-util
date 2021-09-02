# Aviation Calc Util

A library to assist with complex aviation-related calculations. Primarily used for the VATSIM ATC Training Simulator
Project. The library is written in C++ 20 standard and requires a compiler that is capable of building to this standard.

## Overview

The library contains utilities to handle geographical calculations, magnetic calculations, and some math utilties.

## Dependencies

The project depends on the following libraries and uses the `conan` dependency manager.

- [Boost 1.76.0](https://www.boost.org/) (Fetched through Conan)
- [ecCodes 2.22.1](https://confluence.ecmwf.int/display/ECC) (See Below)

### ecCodes

As ecCodes has no Conan or NuGet package, it must be built manually. Fortunately, a `conanfile.py` has been included in
the `eccodes` directory.

To install the package data, simply run `conan export` in the `eccodes` directory.

## Building

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

### NuGet Package

Simply run `create-nuget-package.bat` to generate a NuGet package. The default feed is set
to `%USERPROFILE%\.nuget\feed`.

Overriding default feed can be accomplished by passing your desired feed in as an argument: \
`create-nuget-package.bat S:\data\nugetfeed`

Now the project will be available in Visual Studio as a NuGet package