use std::f64::consts::PI;

use crate::units::angle::Angle;


/// Represents a longitude on a globe
#[derive(Clone, Copy, Default, PartialEq, Debug)]
pub struct Longitude(Angle);

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

    /// Creates a new longitude from rads (radians).
    pub fn from_radians(val: f64) -> Longitude {
        return Self::new(Angle::new(val));
    }

    /// Creates a new longitude from ° (degrees).
    pub fn from_degrees(val: f64) -> Longitude {
        return Self::new(Angle::from_degrees(val));
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
/// use aviation_calc_util::geo::longitude;
/// use aviation_calc_util::units::angle::Angle;
///
/// let calculated = longitude::normalize_longitude(Angle::from_degrees(190.0));
/// let expected = Angle::from_degrees(-170.0);
/// assert!(f64::abs(calculated.as_radians() - expected.as_radians()) < 1.0);
/// ```
pub fn normalize_longitude(lon: Angle) -> Angle {
    return ((lon + (3.0 * PI)) % (2.0 * PI)) - PI;
}