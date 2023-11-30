use std::f64::consts::FRAC_PI_2;

use crate::units::angle::Angle;


/// Represents a latitude on a globe
#[derive(Clone, Copy, Default, PartialEq, Debug)]
pub struct Latitude(Angle);

// Display
impl std::fmt::Display for Latitude {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        return write!(f, "{}°", self.0.as_degrees());
    }
}

// Custom functions
impl Latitude {
    /// Creates a new latitude.
    pub fn new(val: Angle) -> Latitude {
        return Latitude(Angle::new(f64::min(f64::max(val.as_radians(), -FRAC_PI_2), FRAC_PI_2)));
    }

    /// Creates a new latitude from rads (radians).
    pub fn from_radians(val: f64) -> Latitude {
        return Self::new(Angle::new(val));
    }

    /// Creates a new latitude from ° (degrees).
    pub fn from_degrees(val: f64) -> Latitude {
        return Self::new(Angle::from_degrees(val));
    }

    /// Gets the latitude as an Angle
    pub fn as_angle(self) -> Angle {
        return self.0;
    }

    /// Gets the latitude in rads (radians).
    pub fn as_radians(self) -> f64 {
        return self.0.as_radians();
    }

    /// Gets the latitude in ° (degrees).
    pub fn as_degrees(self) -> f64 {
        return self.0.as_degrees();
    }
}