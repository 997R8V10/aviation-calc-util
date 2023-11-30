use std::f64::consts::PI;

use crate::units::angle::Angle;


/// Represents a longitude on a globe
#[derive(Clone, Copy, Default, PartialEq, Debug)]
pub struct Longitude(pub(crate) Angle);

// Display
impl std::fmt::Display for Longitude {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        return write!(f, "{}°", self.0.as_degrees());
    }
}

// Custom functions
impl Longitude {
    /// Creates a new longitude.
    pub fn new(val: Angle) -> Longitude {
        return Longitude(normalize_longitude(val));
    }
    
    /// Gets the longitude in rads (radians).
    pub fn as_radians(self) -> f64 {
        return self.0.as_radians();
    }

    /// Gets the longitude in ° (degrees).
    pub fn as_degrees(self) -> f64 {
        return self.0.as_degrees();
    }

    /// Gets the longitude as an Angle
    pub fn as_angle(self) -> Angle {
        return self.0;
    }

    /// Sets the longitude
    pub fn set_angle(&mut self, val: Angle) {
        self.0 = normalize_longitude(val);
    }
}

/// Normalizes Longitude.
/// 
/// **Parameters:**
/// - `lon` - Input Longitude
/// 
/// **Returns:** \
/// Normalized Longitude
/// 
/// ## Example
///
/// ```
/// use aviation_calc_util::geo_tools::longitude;
/// use aviation_calc_util::units::angle::Angle;
///
/// assert_eq!(geo_util::normalize_longitude(Angle::from_degrees(190.0)), Angle::from_degrees(170.0));
/// ```
pub fn normalize_longitude(lon: Angle) -> Angle {
    return ((lon + (3.0 * PI)) % (2.0 * PI)) - PI;
}