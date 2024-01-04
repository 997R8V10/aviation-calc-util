use std::ops::Add;
use std::ops::Div;
use std::ops::Mul;
use std::ops::Neg;
use std::ops::Rem;
use std::ops::Sub;

use crate::impl_one_part_op_for_number;
use crate::impl_two_part_op_for_number;

use super::Velocity;
use super::unit::Unit;

/// Represents an acceleration quantity.
#[derive(Clone, Copy, Default, PartialEq, PartialOrd, Debug)]
pub struct Acceleration(pub(crate) f64);

// Operator implementations
impl_one_part_op_for_number!(Acceleration, Neg, neg, -);
impl_two_part_op_for_number!(Acceleration, Add, add, +);
impl_two_part_op_for_number!(Acceleration, Sub, sub, -);
impl_two_part_op_for_number!(Acceleration, Mul, mul, *);
impl_two_part_op_for_number!(Acceleration, Div, div, /);
impl_two_part_op_for_number!(Acceleration, Rem, rem, %);

// Display
impl std::fmt::Display for Acceleration {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        return write!(f, "{}m/s^2", self.0);
    }
}

impl Mul<std::time::Duration> for Acceleration {
    type Output = Velocity;

    fn mul(self, rhs: std::time::Duration) -> Self::Output {
        return Velocity::new(self.0 * rhs.as_secs_f64());
    }
}

impl Unit for Acceleration {
    /// Creates a new Acceleration.
    ///
    /// **Parameters:**
    /// - `val` - Value in m/s^2 (meters per second squared)
    fn new(val: f64) -> Acceleration {
        return Acceleration(val);
    }

    /// Gets the velocity in m/s^2 (meters per second squared).
    fn value(&self) -> f64 {
        return self.0;
    }
}

impl From<f64> for Acceleration {
    fn from(value: f64) -> Self {
        return Self::new(value);
    }
}

impl From<Acceleration> for f64 {
    fn from(value: Acceleration) -> Self {
        return value.value();
    }
}

// Custom functions
impl Acceleration {
    /// Creates a new Acceleration.
    ///
    /// **Parameters:**
    /// - `val` - Value in m/s^2 (meters per second squared)
    pub fn from_meters_per_second_squared(val: f64) -> Acceleration {
        return Acceleration(val);
    }

    /// Creates a new Acceleration.
    ///
    /// **Parameters:**
    /// - `val` - Value in kts/s (knots per second, nautical miles per hour per second)
    pub fn from_knots_per_second(val: f64) -> Acceleration {
        return Acceleration(super::Velocity::convert_kts_to_mpers(val));
    }

    /// Gets the velocity in m/s^2 (meters per second squared).
    pub fn as_meters_per_second_squared(&self) -> f64 {
        return self.0;
    }

    /// Gets the velocity in kts/s (knots per second, nautical miles per hour per second)
    pub fn as_knots_per_second(&self) -> f64 {
        return super::Velocity::convert_mpers_to_kts(self.0);
    }
}
