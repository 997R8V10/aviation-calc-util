use std::ops::Add;
use std::ops::Div;
use std::ops::Mul;
use std::ops::Neg;
use std::ops::Rem;
use std::ops::Sub;

use crate::impl_one_part_op_for_number;
use crate::impl_two_part_op_for_number;

use super::Acceleration;
use super::length::Length;
use super::unit::Unit;

/// Represents a velocity quantity.
#[derive(Clone, Copy, Default, PartialEq, PartialOrd, Debug)]
pub struct Velocity(pub(crate) f64);

// Operator implementations
impl_one_part_op_for_number!(Velocity, Neg, neg, -);
impl_two_part_op_for_number!(Velocity, Add, add, +);
impl_two_part_op_for_number!(Velocity, Sub, sub, -);
impl_two_part_op_for_number!(Velocity, Mul, mul, *);
impl_two_part_op_for_number!(Velocity, Div, div, /);
impl_two_part_op_for_number!(Velocity, Rem, rem, %);

// Display
impl std::fmt::Display for Velocity {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        return write!(f, "{}m/s", self.0);
    }
}

impl Mul<std::time::Duration> for Velocity {
    type Output = Length;

    fn mul(self, rhs: std::time::Duration) -> Self::Output {
        return Length::new(self.0 * rhs.as_secs_f64());
    }
}

impl Div<std::time::Duration> for Velocity {
    type Output = Acceleration;

    fn div(self, rhs: std::time::Duration) -> Self::Output {
        return Acceleration::new(self.0 / rhs.as_secs_f64());
    }
}

impl Unit for Velocity {
    /// Creates a new Velocity.
    ///
    /// **Parameters:**
    /// - `val` - Value in m/s (meters per second)
    fn new(val: f64) -> Velocity {
        return Velocity(val);
    }

    /// Gets the velocity in m/s (meters per second).
    fn value(&self) -> f64 {
        return self.0;
    }
}

impl From<f64> for Velocity {
    fn from(value: f64) -> Self {
        return Self::new(value);
    }
}

impl From<Velocity> for f64 {
    fn from(value: Velocity) -> Self {
        return value.value();
    }
}

// Custom functions
impl Velocity {
    /// Creates a new Velocity.
    ///
    /// **Parameters:**
    /// - `val` - Value in m/s (meters per second)
    pub fn from_meters_per_second(val: f64) -> Velocity {
        return Velocity(val);
    }

    /// Creates a new Velocity.
    ///
    /// **Parameters:**
    /// - `val` - Value in kts (knots, nautical miles per hour)
    pub fn from_knots(val: f64) -> Velocity {
        return Velocity(Self::convert_kts_to_mpers(val));
    }

    /// Creates a new Velocity.
    ///
    /// **Parameters:**
    /// - `val` - Value in ft/min (feet per minute)
    pub fn from_feet_per_minute(val: f64) -> Velocity {
        return Velocity(Self::convert_ftpermin_to_mpers(val));
    }

    /// Gets the velocity in m/s (meters per second).
    pub fn as_meters_per_second(&self) -> f64 {
        return self.0;
    }

    /// Gets the velocity in kts (knots, nautical miles per hour)
    pub fn as_knots(&self) -> f64 {
        return Self::convert_mpers_to_kts(self.0);
    }

    /// Gets the velocity in ft/min (feet per minute)
    pub fn as_feet_per_minute(&self) -> f64 {
        return Self::convert_mpers_to_ftpermin(self.0);
    }

    // Constants
    /// Conversion factor from m/s (meters per second) to kts (knots, nautical miles per hour)
    pub const CONV_FACTOR_MPERS_KTS: f64 = 1.943844;

    // Conversion Functions
    /// Convert m/s (meters per second) to kts (knots, nautical miles per hour)
    pub fn convert_mpers_to_kts(mpers: f64) -> f64 {
        return mpers * Self::CONV_FACTOR_MPERS_KTS;
    }

    /// Convert kts (knots, nautical miles per hour) to m/s (meters per second)
    pub fn convert_kts_to_mpers(kts: f64) -> f64 {
        return kts / Self::CONV_FACTOR_MPERS_KTS;
    }

    /// Convert m/s (meters per second) to ft/min (feet per minute)
    pub fn convert_mpers_to_ftpermin(mpers: f64) -> f64 {
        return Length::convert_meters_to_feet(60.0 * mpers);
    }

    /// Convert ft/min (feet per minute) to m/s (meters per second)
    pub fn convert_ftpermin_to_mpers(ftpermin: f64) -> f64 {
        return Length::convert_feet_to_meters(ftpermin) / 60.0;
    }
}
