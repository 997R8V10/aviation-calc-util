use std::ops::Add;
use std::ops::Div;
use std::ops::Mul;
use std::ops::Neg;
use std::ops::Rem;
use std::ops::Sub;

use crate::impl_one_part_op_for_number;
use crate::impl_two_part_op_for_number;

use super::angle::Angle;
use super::unit::Unit;

/// Represents a angular velocity quantity.
#[derive(Clone, Copy, Default, PartialEq, PartialOrd, Debug)]
pub struct AngularVelocity(pub(crate) f64);

// Operator implementations
impl_one_part_op_for_number!(AngularVelocity, Neg, neg, -);
impl_two_part_op_for_number!(AngularVelocity, Add, add, +);
impl_two_part_op_for_number!(AngularVelocity, Sub, sub, -);
impl_two_part_op_for_number!(AngularVelocity, Mul, mul, *);
impl_two_part_op_for_number!(AngularVelocity, Div, div, /);
impl_two_part_op_for_number!(AngularVelocity, Rem, rem, %);

// Display
impl std::fmt::Display for AngularVelocity {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        return write!(f, "{}rad/s", self.0);
    }
}

impl Mul<std::time::Duration> for AngularVelocity {
    type Output = Angle;

    fn mul(self, rhs: std::time::Duration) -> Self::Output {
        return Angle::new(self.0 * rhs.as_secs_f64());
    }
}

impl Unit for AngularVelocity {
    /// Creates a new Angular Velocity.
    ///
    /// **Parameters:**
    /// - `val` - Value in rad/s (radians per second)
    fn new(val: f64) -> AngularVelocity {
        return AngularVelocity(val);
    }

    /// Gets the angular velocity in rad/s (radians per second).
    fn value(&self) -> f64 {
        return self.0;
    }
}

impl From<f64> for AngularVelocity {
    fn from(value: f64) -> Self {
        return Self::new(value);
    }
}

impl From<AngularVelocity> for f64 {
    fn from(value: AngularVelocity) -> Self {
        return value.value();
    }
}

// Custom functions
impl AngularVelocity {
    /// Creates a new Angular Velocity.
    ///
    /// **Parameters:**
    /// - `val` - Value in rad/s (radians per second)
    pub fn from_radians_per_second(val: f64) -> AngularVelocity {
        return AngularVelocity(val);
    }

    /// Creates a new Angular Velocity.
    ///
    /// **Parameters:**
    /// - `val` - Value in °/s (degrees per second)
    pub fn from_degrees_per_second(val: f64) -> AngularVelocity {
        return AngularVelocity(val.to_radians());
    }

    /// Gets the angular velocity in rad/s (radians per second).
    pub fn as_radians_per_second(&self) -> f64 {
        return self.0;
    }

    /// Gets the angular velocity in °/s (degrees per second).
    pub fn as_degrees_per_second(&self) -> f64 {
        return self.0.to_degrees();
    }
}