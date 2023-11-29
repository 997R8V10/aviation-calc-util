// Constants
pub const EARTH_RADIUS_M: f64 = 6371e3;

pub trait GeoTrait {
    fn normalize_longitude(self) -> Self;
    fn normalize_heading(self) -> Self;
}

impl GeoTrait for f64 {
    fn normalize_longitude(self) -> Self {
        return normalize_longitude(self);
    }

    fn normalize_heading(self) -> Self {
        return normalize_heading(self);
    }
}

/// Normalizes Longitude between -180 and +180 degrees.
/// 
/// **Parameters:**
/// - `lon` - Input Longitude (degrees)
/// 
/// **Returns:** \
/// Normalized Longitude (degrees)
/// 
/// ## Example
///
/// ```
/// use aviation_calc_util::geo_tools::geo_util;
///
/// assert_eq!(geo_util::normalize_longitude(-190.0), 170.0);
/// ```
pub fn normalize_longitude(lon: f64) -> f64 {
    return ((lon + 540.0) % 360.0) - 180.0;
}

/// Normalizes Heading between 0 and 360 degrees.
/// 
/// **Parameters:**
/// - `hdg` - Input Heading (degrees)
/// 
/// **Returns:** \
/// Normalized Heading (degrees)
/// 
/// ## Example
///
/// ```
/// use aviation_calc_util::geo_tools::geo_util;
///
/// assert_eq!(geo_util::normalize_heading(370.0), 10.0);
/// ```
pub fn normalize_heading(hdg: f64) -> f64 {
    return (hdg + 360.0) % 360.0;
}