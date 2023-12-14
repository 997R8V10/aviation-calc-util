use std::ops::Add;
use std::ops::Div;
use std::ops::Mul;
use std::ops::Neg;
use std::ops::Rem;
use std::ops::Sub;

use crate::impl_one_part_op_for_number;
use crate::impl_two_part_op_for_number;

use super::unit::Unit;
use super::velocity::Velocity;

/// Represents a length quantity.
#[derive(Clone, Copy, Default, PartialEq, PartialOrd, Debug)]
pub struct Length(pub(crate) f64);

// Operator implementations
impl_one_part_op_for_number!(Length, Neg, neg, -);
impl_two_part_op_for_number!(Length, Add, add, +);
impl_two_part_op_for_number!(Length, Sub, sub, -);
impl_two_part_op_for_number!(Length, Mul, mul, *);
impl_two_part_op_for_number!(Length, Div, div, /);
impl_two_part_op_for_number!(Length, Rem, rem, %);

// Display
impl std::fmt::Display for Length {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        return write!(f, "{}m", self.0);
    }
}

impl Div<std::time::Duration> for Length {
    type Output = Velocity;

    fn div(self, rhs: std::time::Duration) -> Self::Output {
        return Velocity::new(self.0 / rhs.as_secs_f64());
    }
}

impl Unit for Length {
    /// Creates a new Length.
    ///
    /// **Parameters:**
    /// - `val` - Value in m (meters)
    fn new(val: f64) -> Length {
        return Length(val);
    }

    /// Gets the length in m (meters).
    fn value(&self) -> f64 {
        return self.0;
    }
}

impl From<f64> for Length {
    fn from(value: f64) -> Self {
        return Self::new(value);
    }
}

impl From<Length> for f64 {
    fn from(value: Length) -> Self {
        return value.value();
    }
}

// Custom functions
impl Length {
    /// Creates a new Length.
    ///
    /// **Parameters:**
    /// - `val` - Value in m (meters)
    pub fn from_meters(val: f64) -> Length {
        return Length(val);
    }

    /// Creates a new Length.
    ///
    /// **Parameters:**
    /// - `val_feet` - Value in ft (feet)
    pub fn from_feet(val_feet: f64) -> Length {
        return Length(Self::convert_feet_to_meters(val_feet));
    }

    /// Creates a new Length.
    ///
    /// **Parameters:**
    /// - `val_nautical_miles` - Value in nmi (nautical miles)
    pub fn from_nautical_miles(val_nautical_miles: f64) -> Length {
        return Length(Self::convert_nautical_miles_to_meters(val_nautical_miles));
    }

    /// Creates a new Length.
    ///
    /// **Parameters:**
    /// - `val_statute_miles` - Value in mi (statute miles)
    pub fn from_statute_miles(val_statute_miles: f64) -> Length {
        return Length(Self::convert_statute_miles_to_meters(val_statute_miles));
    }

    /// Gets the length in m (meters).
    pub fn as_meters(&self) -> f64 {
        return self.0;
    }

    /// Gets the length in ft (feet).
    pub fn as_feet(&self) -> f64 {
        return Self::convert_meters_to_feet(self.0);
    }

    /// Gets the length in mi (statute miles).
    pub fn as_statute_miles(&self) -> f64 {
        return Self::convert_meters_to_statute_miles(self.0);
    }

    /// Gets the length in nmi (nautical miles).
    pub fn as_nautical_miles(&self) -> f64 {
        return Self::convert_meters_to_nautical_miles(self.0);
    }

    // Constants
    /// Conversion factor from m (meters) to ft (feet)
    pub const CONV_FACTOR_M_FT: f64 = 3.28084;
    /// Conversion factor from nmi (nautical miles) to m (meters)
    pub const CONV_FACTOR_NMI_M: f64 = 1852.0;
    /// Conversion factor from mi (statute miles) to m (meters)
    pub const CONV_FACTOR_MI_M: f64 = 1609.344;

    // Unit Conversion Functions
    /// Convert m (meters) to ft (feet)
    pub fn convert_meters_to_feet(meters: f64) -> f64 {
        return meters * Self::CONV_FACTOR_M_FT;
    }

    /// Convert m (meters) to nmi (nautical miles)
    pub fn convert_meters_to_nautical_miles(meters: f64) -> f64 {
        return meters / Self::CONV_FACTOR_NMI_M;
    }

    /// Convert m (meters) to mi (statute miles)
    pub fn convert_meters_to_statute_miles(meters: f64) -> f64 {
        return meters / Self::CONV_FACTOR_MI_M;
    }

    /// Convert ft (feet) to m (meters)
    pub fn convert_feet_to_meters(feet: f64) -> f64 {
        return feet / Self::CONV_FACTOR_M_FT;
    }

    /// Convert nmi (nautical miles) to m (meters)
    pub fn convert_nautical_miles_to_meters(nmi: f64) -> f64 {
        return nmi * Self::CONV_FACTOR_NMI_M;
    }

    /// Convert mi (statute miles) to m (meters)
    pub fn convert_statute_miles_to_meters(mi: f64) -> f64 {
        return mi * Self::CONV_FACTOR_MI_M;
    }
}
