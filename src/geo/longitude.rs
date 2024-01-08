use std::f64::consts::PI;

use crate::units::{Angle, Unit};

use super::{convert_decimal_deg_to_vrc_single, convert_decimal_to_nats_single, convert_nats_to_decimal_single, convert_vrc_to_decimal_deg_single};

/// Represents a longitude on a globe
#[derive(Clone, Copy, Default, PartialEq, PartialOrd)]
pub struct Longitude(Angle);

// Display
impl std::fmt::Display for Longitude {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        return write!(f, "{}°", self.0.as_degrees());
    }
}

impl std::fmt::Debug for Longitude {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        return std::fmt::Display::fmt(&self, f);
    }
}


impl From<f64> for Longitude {
    /// Creates a new Longitude from ° (degrees).
    fn from(value: f64) -> Self {
        return Longitude::from_degrees(value);
    }
}

impl From<Longitude> for f64 {
    /// Gets the Longitude in ° (degrees).
    fn from(value: Longitude) -> Self {
        return value.as_degrees();
    }
}

impl From<Angle> for Longitude {
    fn from(value: Angle) -> Self {
        return Longitude::new(value);
    }
}

impl From<Longitude> for Angle {
    fn from(value: Longitude) -> Self {
        return value.as_angle();
    }
}

// Custom functions
impl Longitude {
    /// Creates a new longitude.
    pub fn new(val: Angle) -> Longitude {
        return Longitude(Self::normalize_longitude(val));
    }

    /// Creates a new longitude from rads (radians).
    pub fn from_radians(val: f64) -> Longitude {
        return Self::new(Angle::new(val));
    }

    /// Creates a new longitude from ° (degrees).
    pub fn from_degrees(val: f64) -> Longitude {
        return Self::new(Angle::from_degrees(val));
    }

    /// Creates a new longitude from degrees mins and secs
    pub fn from_deg_min_sec(deg: i32, mins: u32, secs: f64) -> Longitude {
        return Longitude::new(Angle::from_deg_min_sec(deg, mins, secs));
    }

    /// Creates a new longitude from SCT format
    /// 
    /// String should be formatted like `W049.52.27.771`
    /// 
    /// ## Example
    ///
    /// ```
    /// use aviation_calc_util::geo::Longitude;
    ///
    /// let calculated = Longitude::from_vrc("W52.34.12.123").expect("Could not convert VRC String");
    /// let expected = Longitude::from_deg_min_sec(-52, 34 as u32, 12.123 as f64);
    /// assert!(f64::abs(calculated.as_radians() - expected.as_radians()) < 0.1);
    /// ```
    pub fn from_vrc(vrc_coord: &str) -> Option<Longitude> {
        if let Some(angle) = convert_vrc_to_decimal_deg_single(vrc_coord) {
            return Some(Longitude::new(angle));
        }

        return None;
    }

    /// Creates a new longitude from the NATS format used in AIPs.
    /// 
    /// ## Example
    ///
    /// ```
    /// use aviation_calc_util::geo::Longitude;
    ///
    /// let calculated = Longitude::from_nats("0061347W").expect("Could not convert NATS String");
    /// let expected = Longitude::from_deg_min_sec(-6, 13 as u32, 47 as f64);
    /// assert!(f64::abs(calculated.as_radians() - expected.as_radians()) < 0.1);
    /// ```
    pub fn from_nats(nats_coord: &str) -> Option<Longitude> {
        if let Some(angle) = convert_nats_to_decimal_single(nats_coord, false) {
            return Some(Longitude::new(angle));
        }

        return None;
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

    /// Gets the longitude in degrees, mins, secs
    pub fn as_deg_min_sec(&self) -> (i32, u32, f64) {
        return self.0.as_deg_min_sec();
    }

    /// Gets the longitude as a SCT string
    pub fn as_vrc(&self) -> String {
        return convert_decimal_deg_to_vrc_single(self.0, false);
    }

    /// Gets the longitude as a NATS string
    pub fn as_nats(&self) -> String {
        return convert_decimal_to_nats_single(self.0, false);
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
    /// use aviation_calc_util::geo::Longitude;
    /// use aviation_calc_util::units::Angle;
    ///
    /// let calculated = Longitude::normalize_longitude(Angle::from_degrees(190.0));
    /// let expected = Angle::from_degrees(-170.0);
    /// assert!(f64::abs(calculated.as_radians() - expected.as_radians()) < 1.0);
    /// ```
    pub fn normalize_longitude(lon: Angle) -> Angle {
        return ((lon + (3.0 * PI)) % (2.0 * PI)) - PI;
    }
}


