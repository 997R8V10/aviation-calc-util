# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [2.13.2] - 2024-12-14
### Fixed
- `serde` `Angle` deserialization for integer types.

## [2.13.1] - 2024-12-09
### Fixed
- `serde` `GeoPoint` deserialization default altitude.

## [2.13.0] - 2024-12-07
### Added
- `serde` serialization/deserialization

## [2.12.1] - 2024-01-12
### Fixed
- `grib`
  - Grib Tile rounding error

## [2.12.0] - 2024-01-10
### Added
- `aviation`
  - Calculate Turn Rate function

## [2.11.8] - 2024-01-10
### Added
- `tests`
  - Multiple Unit tests

### Fixed
- `geo/bearing`
  - Subtracting two Bearings was wrong
- `aviation`
  - Chord Line Calculations for left turns

## [2.11.5] - 2024-01-09
### Fixed
- `magnetic`
  - Fixed MAJOR magnetic calculation issues

## [2.11.3] - 2024-01-09
### Fixed
- `math`
  - Fixed type in interpolate

## [2.11.1] - 2024-01-08
### Fixed
- `geo`
  - Lat/Lon from VRC coord conversion

## [2.11.0] - 2024-01-06
### Added
- `units`
  - Percentage as a unit for angles

## [2.10.0] - 2024-01-05
### Added
- `physics`
  - Acceleration kinematics equation

## [2.9.0] - 2024-01-04
### Changed
- `geo`
  - Earth Gravity to an Acceleration
  
## [2.8.1] - 2024-01-04
### Added
- `physics`
  - Kinematics Equations
- `math`
  - Interoplation Function
  - Line intersections
- `units`
  - `acceleration`
    - New Acceleration Quantity
  - `length`
    - Length / Velocity => Time
  - `velocity`
    - Velocity / Time => Acceleration

## [2.7.2] - 2023-12-31
### Fixed
- `geo`
  - `Longitude`
    - Fixed NATS conversion functions

## [2.7.0] - 2023-12-16
### Added
- `units`
  - `Angle`
    - Added Arc Length functions
- `aviation`
  - Added FPA conversion functions

## [2.6.0] - 2023-12-14
### Fixed
- MagneticTile pass MagneticModel by reference.

## [2.5.0] - 2023-12-14
### Changed
- MagneticTileManager now allows for calculations with no Magnetic Model.

## [2.4.0] - 2023-12-14
### Added
- Casts from f64 to units

### Removed
- Math functions from units. Cast to f64.

## [2.3.3] - 2023-12-13
### Fixed
- Multiple typos that caused errors

## [2.3.2] - 2023-12-13
### Changed
- Moved `atmos`/`grib` structs to their own file.

## [2.3.0] - 2023-12-12
### Changed
- `atmos`/`grib`
  - `GribTile` now returned as a `Arc<Mutex<GribTile>>` to allow for mutability

## [2.2.1] - 2023-12-12
### Fixed
- `atmos`
  - `calculate_dry_air_density` now takes a `Pressure` instead of a `f64`

## [2.2.0] - 2023-12-11
### Changed
- Re-arranged to make imports easier

### Fixed
- Crate now re-exports anyhow Result and Error
- Added more documentation

## [2.1.1] - 2023-12-11
### Fixed
- Crate now re-exports chrono Datelike trait

## [2.1.0] - 2023-12-11
### Added
- Unit trait to encompass most math functions

### Removed
- Assignment operators for Units

## [2.0.0] - 2023-12-08
**MAJOR CHANGE!!**: Library moved from C++ to Rust.

### Added
- Unit types to make managing units easier

### Changes
- Entire library moved to Rust from C++.

### Removed
- FFI (C) bindings moved to separate library
 
## [1.2.0] - 2023-11-09
### Added
- GeoPoint
  - Find Closest Intersection

### Fixed
- Perl Dependency Fetching
- GeoPoint
  - Distance Haversine formula not working for points on opposite ends of globe.

## [1.1.2] - 2023-10-15
### Fixed
- Arc Course Calculation
  - Use final bearing to respect great-circle

## [1.1.1] - 2023-10-15
### Fixed
- FFI Arc Course Calculations function name.

## [1.1.0] - 2023-10-15
### Added
- Radius to Fix (Arc) course calculations.
- Calculate delta to heading with turn direction function.

## [1.0.20] - 2023-10-14
### Added
- Github Auto-release

### Fixed
- Github Actions Nuget Push
- Conan Cache for Github Runner
- NuGet Targets runtimes folder path
- Github Actions conan profile detect issue
- AnyCPU Platform resolved automatically

## [1.0.16] - 2023-10-13
### Changed
- Create 1 nuget package with all native deps inside.
- Upgraded to Conan 2.0

## [1.0.12] - 2023-02-17
### Fixed
- Removed Curl dependency for MacOS

## [1.0.11] - 2023-02-17
### Fixed
- Fixed RPATH issues on Linux and MacOS with eccodes package.
- Update eccodes package with 'p1' for patch 1.

## [1.0.9] - 2023-02-16
### Fixed
- Fixed libcurl dependency issues. Now included with Conan

## [1.0.8] - 2023-02-16
### Fixed
- Fixed CI/CD On Non-Windows Builds with copying binaries

## [1.0.7] - 2023-02-16
### Fixed
- Fixed CI/CD Symbols Packages

## [1.0.5] - 2023-02-15
### Added
- Added CI/CD

## [1.0.4] - 2023-02-13
### Fixed
- Added LICENSE and README to NuGet Package

## [1.0.3] - 2023-02-13
### Fixed
- Fixed NuGet Package Generation

## [1.0.2] - 2023-02-12
### Changed
- Generating separate NuGet Packages per architecture

## [1.0.0] - 2023-02-06
### Added
- Coordinate Utilities
  - Added functions to convert between decimal degrees and DDD.MM.SS.SSS.
  - Added functions to convert from NATS/VRC/Euroscope coordinates to decimal and back.

### Changed
- Moved Atmospheric Functions from GeoUtil to AtmosUtil

### Removed
- Deprecated GeoUtil Atmospheric Functions
  - `convertPressureToDensityAlt`
  - `convertIasToTas`
  - `convertTasToIas`

### Fixed
- Static Library Builds
  - Added batch file.
  - Fixed EcCodes MemFS not linking.
  - Fixed issue with static libs not being built.

## [0.3.6] - 2021-11-04
### Fixed
- Release builds were being built as debug on Windows.

## [0.3.5] - 2021-09-19
### Fixed
- ecCodes: Fixed erroneous perl dependency in conanfile.

## [0.3.4] - 2021-09-19
### Fixed
- Corrected issue with GeoPoint setters only setting latitude.

## [0.3.3] - 2021-09-16
### Fixed
- Issues on MacOS with Filesystem linker flags.

## [0.3.2] - 2021-09-16
### Fixed
- Errors on Ubuntu with Boost::Filesystem.
  - Boost Filesystem replaced with std::filesystem
- Added exception handling to GribTile

## [0.3.1] - 2021-09-15
### Fixed
- Targeting .NETStandard2.0 for NuGet package.

## [0.3.0] - 2021-09-15
### Added
- C api for all functions for .NET and Swift support.
- NuGet package creation on Linux.
- Copy Constructors added to several classes.
- NuGet package now targets Native, .NET Core App 3.1, and .NET Framework 4.7

## [0.2.1] - 2021-09-03
### Fixed
- Geo Tools
  - AtmosUtil
    - NaN result from TAS->IAS calculation fixed.

## [0.2.0] - 2021-09-03
### Added
- Geo Tools
  - Atmospheric Utilities (AtmosUtil)
    - Added more accurate functions to calculate various atmospheric parameters.
- Math Tools
  - MathUtil
    - Added temperature and velocity conversions.

### Deprecated
- Geo Tools
  - GeoUtil
    - Use the functions in AtmosUtil
      - `convertIasToTas`
      - `convertTasToIas`
      - `convertedPressureToDensityAlt`

### Fixed
- Magnetic Tools
  - MagneticResult
    - Fixed issue with fields being null

## [0.1.0] - 2021-09-02
### Added
- Geo Tools
  - GeoTile
    - To consolidate shared tile code
- Magnetic Tools
  - MagneticTile
    - To reduce frequency of magnetic calculations by storing the variation in 0.1 degree tiles.
- Changelog

### Changed
- Geo Tools
  - Grib Tools
    - GribTile
      - Now uses GeoPoint to consolidate tile code. Does not affect usage.
- Magnetic Tools
  - MagneticUtil
    - **BREAKING**: `getMagneticField()` now returns a `shared_ptr` instead of a `unique_ptr`!
    - Added 2 functions to convert from True <=> Magnetic that now use `MagneticTile` for improved performance.

## [0.0.7] - 2021-09-02
### Added
- GeoUtil
  - Indicated to True Airspeed Function
  - True to Indicated Airspeed Function
- ReadMe

### Changed
- GribTile
  - ecCodes now stores definitions in memory

### Fixed
- NuGet package now builds correctly and automatically deploys to local NuGet feed

## [0.0.1] - 2021-09-01
### Added
- Geo Tools
  - Geographical Utilities
  - GeoPoint
    - Class to store a world point
    - Haversine formulae for great circle calculations
  - Grib Tools
    - Using ecCodes Library
  - Magnetic Tools
    - Using WMM data
- Math Tools
  - Conversion Functions
  - Legendre Spherical Harmonics Functions (For Magnetic Calculations)

[2.13.2]: https://github.com/997R8V10/aviation-calc-util/compare/v2.13.1...v2.13.2
[2.13.1]: https://github.com/997R8V10/aviation-calc-util/compare/v2.13.0...v2.13.1
[2.13.0]: https://github.com/997R8V10/aviation-calc-util/compare/v2.12.1...v2.13.0
[2.12.1]: https://github.com/997R8V10/aviation-calc-util/compare/v2.12.0...v2.12.1
[2.12.0]: https://github.com/997R8V10/aviation-calc-util/compare/v2.11.8...v2.12.0
[2.11.8]: https://github.com/997R8V10/aviation-calc-util/compare/v2.11.5...v2.11.8
[2.11.5]: https://github.com/997R8V10/aviation-calc-util/compare/v2.11.3...v2.11.5
[2.11.3]: https://github.com/997R8V10/aviation-calc-util/compare/v2.11.1...v2.11.3
[2.11.1]: https://github.com/997R8V10/aviation-calc-util/compare/v2.11.0...v2.11.1
[2.11.0]: https://github.com/997R8V10/aviation-calc-util/compare/v2.10.0...v2.11.0
[2.10.0]: https://github.com/997R8V10/aviation-calc-util/compare/v2.9.0...v2.10.0
[2.9.0]: https://github.com/997R8V10/aviation-calc-util/compare/v2.8.1...v2.9.0
[2.8.1]: https://github.com/997R8V10/aviation-calc-util/compare/v2.7.2...v2.8.1
[2.7.2]: https://github.com/997R8V10/aviation-calc-util/compare/v2.7.0...v2.7.2
[2.7.0]: https://github.com/997R8V10/aviation-calc-util/compare/v2.6.0...v2.7.0
[2.6.0]: https://github.com/997R8V10/aviation-calc-util/compare/v2.5.0...v2.6.0
[2.5.0]: https://github.com/997R8V10/aviation-calc-util/compare/v2.4.0...v2.5.0
[2.4.0]: https://github.com/997R8V10/aviation-calc-util/compare/v2.3.4...v2.4.0
[2.3.3]: https://github.com/997R8V10/aviation-calc-util/compare/v2.3.2...v2.3.3
[2.3.2]: https://github.com/997R8V10/aviation-calc-util/compare/v2.3.0...v2.3.2
[2.3.0]: https://github.com/997R8V10/aviation-calc-util/compare/v2.2.1...v2.3.0
[2.2.1]: https://github.com/997R8V10/aviation-calc-util/compare/v2.2.0...v2.2.1
[2.2.0]: https://github.com/997R8V10/aviation-calc-util/compare/v2.1.1...v2.2.0
[2.1.1]: https://github.com/997R8V10/aviation-calc-util/compare/v2.1.0...v2.1.1
[2.1.0]: https://github.com/997R8V10/aviation-calc-util/compare/v2.0.0...v2.1.0
[2.0.0]: https://github.com/997R8V10/aviation-calc-util/compare/v1.2.0...v2.0.0
[1.2.0]: https://github.com/997R8V10/aviation-calc-util/compare/v1.1.2...v1.2.0
[1.1.2]: https://github.com/997R8V10/aviation-calc-util/compare/v1.1.1...v1.1.2
[1.1.1]: https://github.com/997R8V10/aviation-calc-util/compare/v1.1.0...v1.1.1
[1.1.0]: https://github.com/997R8V10/aviation-calc-util/compare/v1.0.20...v1.1.0
[1.0.20]: https://github.com/997R8V10/aviation-calc-util/compare/v1.0.16...v1.0.20
[1.0.16]: https://github.com/997R8V10/aviation-calc-util/compare/v1.0.12...v1.0.16
[1.0.12]: https://github.com/997R8V10/aviation-calc-util/compare/v1.0.11...v1.0.12
[1.0.11]: https://github.com/997R8V10/aviation-calc-util/compare/v1.0.9...v1.0.11
[1.0.9]: https://github.com/997R8V10/aviation-calc-util/compare/v1.0.8...v1.0.9
[1.0.8]: https://github.com/997R8V10/aviation-calc-util/compare/v1.0.7...v1.0.8
[1.0.7]: https://github.com/997R8V10/aviation-calc-util/compare/v1.0.5...v1.0.7
[1.0.5]: https://github.com/997R8V10/aviation-calc-util/compare/v1.0.4...v1.0.5
[1.0.4]: https://github.com/997R8V10/aviation-calc-util/compare/v1.0.3...v1.0.4
[1.0.3]: https://github.com/997R8V10/aviation-calc-util/compare/v1.0.2...v1.0.3
[1.0.2]: https://github.com/997R8V10/aviation-calc-util/compare/v1.0.0...v1.0.2
[1.0.0]: https://github.com/997R8V10/aviation-calc-util/compare/v0.3.6...v1.0.0
[0.3.6]: https://github.com/997R8V10/aviation-calc-util/compare/v0.3.5...v0.3.6
[0.3.5]: https://github.com/997R8V10/aviation-calc-util/compare/v0.3.4...v0.3.5
[0.3.4]: https://github.com/997R8V10/aviation-calc-util/compare/v0.3.3...v0.3.4
[0.3.3]: https://github.com/997R8V10/aviation-calc-util/compare/v0.3.2...v0.3.3
[0.3.2]: https://github.com/997R8V10/aviation-calc-util/compare/v0.3.1...v0.3.2
[0.3.1]: https://github.com/997R8V10/aviation-calc-util/compare/v0.3.0...v0.3.1
[0.3.0]: https://github.com/997R8V10/aviation-calc-util/compare/v0.2.1...v0.3.0
[0.2.1]: https://github.com/997R8V10/aviation-calc-util/compare/v0.2.0...v0.2.1
[0.2.0]: https://github.com/997R8V10/aviation-calc-util/compare/v0.1.0...v0.2.0
[0.1.0]: https://github.com/997R8V10/aviation-calc-util/compare/v0.0.7...v0.1.0
[0.0.7]: https://github.com/997R8V10/aviation-calc-util/compare/v0.0.1...v0.0.7
[0.0.1]: https://github.com/997R8V10/aviation-calc-util/releases/tag/v0.0.1

