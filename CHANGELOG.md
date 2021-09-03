# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [0.1.1] - 2021-09-03
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
    - Use the function sin AtmosUtil
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

[Unreleased]: https://gitlab.com/vatsim-tools/aviation-calc-util/-/compare/0.1.0...HEAD
[0.1.0]: https://gitlab.com/vatsim-tools/aviation-calc-util/-/compare/v0.0.7...v0.1.0
[0.0.7]: https://gitlab.com/vatsim-tools/aviation-calc-util/-/compare/v0.0.1...v0.0.7
[0.0.1]: https://gitlab.com/vatsim-tools/aviation-calc-util/-/releases/v0.0.1

