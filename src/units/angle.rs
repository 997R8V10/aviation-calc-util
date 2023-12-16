use std::ops::Add;
use std::ops::Div;
use std::ops::Mul;
use std::ops::Neg;
use std::ops::Rem;
use std::ops::Sub;

use crate::impl_one_part_op_for_number;
use crate::impl_two_part_op_for_number;

use super::Length;
use super::angular_velocity::AngularVelocity;
use super::unit::Unit;

/// Represents a angle quantity.
#[derive(Clone, Copy, Default, PartialEq, PartialOrd, Debug)]
pub struct Angle(pub(crate) f64);

// Operator implementations
impl_one_part_op_for_number!(Angle, Neg, neg, -);
impl_two_part_op_for_number!(Angle, Add, add, +);
impl_two_part_op_for_number!(Angle, Sub, sub, -);
impl_two_part_op_for_number!(Angle, Mul, mul, *);
impl_two_part_op_for_number!(Angle, Div, div, /);
impl_two_part_op_for_number!(Angle, Rem, rem, %);

// Display
impl std::fmt::Display for Angle {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        return write!(f, "{}rads", self.0);
    }
}

impl Div<std::time::Duration> for Angle {
    type Output = AngularVelocity;

    fn div(self, rhs: std::time::Duration) -> Self::Output {
        return AngularVelocity::new(self.0 / rhs.as_secs_f64());
    }
}

impl Unit for Angle {
    /// Creates a new Angle.
    ///
    /// **Parameters:**
    /// - `val` - Value in rads (radians)
    fn new(val: f64) -> Angle {
        return Angle(val);
    }

    /// Gets the angle in rads (radians).
    fn value(&self) -> f64 {
        return self.0;
    }
}

impl From<f64> for Angle {
    fn from(value: f64) -> Self {
        return Self::new(value);
    }
}

impl From<Angle> for f64 {
    fn from(value: Angle) -> Self {
        return value.value();
    }
}

// Custom functions
impl Angle {
    /// Creates a new Angle.
    ///
    /// **Parameters:**
    /// - `val` - Value in rads (radians)
    pub fn from_radians(val: f64) -> Angle {
        return Angle(val);
    }

    /// Creates a new Angle
    ///
    /// **Parameters:**
    /// - `val` - Value in ° (degrees)
    pub fn from_degrees(val: f64) -> Angle {
        return Angle(val.to_radians());
    }

    /// Creates a new Angle from degrees, mins, secs
    pub fn from_deg_min_sec(degrees: i32, mins: u32, secs: f64) -> Angle {
        let sign = if degrees < 0 { -1.0 } else { 1.0 };
        let new_degrees = degrees as f64 * sign;
        let decimal_degs = sign * (new_degrees + (mins as f64 / 60.0) + (secs / 3600.0));

        return Angle::from_degrees(decimal_degs);
    }

    /// Gets the angle in rads (radians).
    pub fn as_radians(&self) -> f64 {
        return self.0;
    }

    /// Gets the angle in ° (degrees).
    pub fn as_degrees(&self) -> f64 {
        return self.0.to_degrees();
    }

    /// Gets the angle in degrees, mins, secs
    pub fn as_deg_min_sec(&self) -> (i32, u32, f64) {
        let sign = if self.0 < 0.0  {-1.0} else {1.0};
        let new_degs = self.as_degrees() * sign;

        let mut degrees = new_degs.trunc();
        let mins = ((new_degs - degrees) * 60.0).trunc();
        let secs = (new_degs - degrees - (mins / 60.0)) * 3600.0;

        degrees *= sign;

        return (degrees as i32, mins as u32, secs);
    }

    pub fn calculate_angle_from_arc_length(arc_length: Length, radius: Length) -> Angle {
        // c = theta * r
        // theta = c / r
        return Angle::new((arc_length / radius).into());
    }
}
