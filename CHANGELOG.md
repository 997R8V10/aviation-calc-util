# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]
### Added
- Magnetic Tools
  - MagneticTile to reduce frequency of magnetic calculations by storing the variation in 1 degree tiles.

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

[Unreleased]: https://gitlab.com/vatsim-tools/aviation-calc-util/-/compare/0.0.7...HEAD
[0.0.7]: https://gitlab.com/vatsim-tools/aviation-calc-util/-/compare/v0.0.1...v0.0.7
[0.0.1]: https://gitlab.com/vatsim-tools/aviation-calc-util/-/releases/v0.0.1

