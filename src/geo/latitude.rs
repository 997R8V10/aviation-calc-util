use std::f64::consts::FRAC_PI_2;

use crate::units::{Angle, Unit};

use super::{convert_vrc_to_decimal_deg_single, convert_nats_to_decimal_single, convert_decimal_deg_to_vrc_single, convert_decimal_to_nats_single};


/// Represents a latitude on a globe
#[derive(Clone, Copy, Default, PartialEq, PartialOrd)]
pub struct Latitude(Angle);

// Display
impl std::fmt::Display for Latitude {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        return write!(f, "{}°", self.0.as_degrees());
    }
}

impl std::fmt::Debug for Latitude {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        return std::fmt::Display::fmt(&self, f);
    }
}

impl From<f64> for Latitude {
    fn from(value: f64) -> Self {
        return Latitude::from_degrees(value);
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

    /// Creates a new latitude from degrees mins and secs
    pub fn from_deg_min_sec(deg: i32, mins: u32, secs: f64) -> Latitude {
        return Latitude::new(Angle::from_deg_min_sec(deg, mins, secs));
    }

    /// Creates a new latitude from SCT format
    /// 
    /// String should be formatted like `N049.52.27.771`
    pub fn from_vrc(vrc_coord: &str) -> Option<Latitude> {
        if let Some(angle) = convert_vrc_to_decimal_deg_single(vrc_coord) {
            return Some(Latitude::new(angle));
        }

        return None;
    }

    /// Creates a new latitude from the NATS format used in AIPs.
    pub fn from_nats(nats_coord: &str) -> Option<Latitude> {
        if let Some(angle) = convert_nats_to_decimal_single(nats_coord, true) {
            return Some(Latitude::new(angle));
        }

        return None;
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

    /// Gets the latitude in degrees, mins, secs
    pub fn as_deg_min_sec(&self) -> (i32, u32, f64) {
        return self.0.as_deg_min_sec();
    }

    /// Gets the latitude as a SCT string
    pub fn as_vrc(&self) -> String {
        return convert_decimal_deg_to_vrc_single(self.0, true);
    }

    /// Gets the latitude as a NATS string
    pub fn as_nats(&self) -> String {
        return convert_decimal_to_nats_single(self.0, true);
    }
}