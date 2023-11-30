use std::ops::Add;
use std::ops::AddAssign;
use std::ops::Div;
use std::ops::DivAssign;
use std::ops::Mul;
use std::ops::MulAssign;
use std::ops::Neg;
use std::ops::Rem;
use std::ops::RemAssign;
use std::ops::Sub;
use std::ops::SubAssign;

use crate::impl_assign_op_for_number;
use crate::impl_num_methods;
use crate::impl_one_part_op_for_number;
use crate::impl_two_part_op_for_number;

use super::angular_velocity::AngularVelocity;

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

// Assignment Operator implementations
impl_assign_op_for_number!(Angle, AddAssign, add_assign, +=);
impl_assign_op_for_number!(Angle, SubAssign, sub_assign, -=);
impl_assign_op_for_number!(Angle, MulAssign, mul_assign, *=);
impl_assign_op_for_number!(Angle, DivAssign, div_assign, /=);
impl_assign_op_for_number!(Angle, RemAssign, rem_assign, %=);

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

// Custom functions
impl Angle {
    /// Creates a new Angle.
    ///
    /// **Parameters:**
    /// - `val` - Value in rads (radians)
    pub fn new(val: f64) -> Angle {
        return Angle(val);
    }

    /// Creates a new Angle
    ///
    /// **Parameters:**
    /// - `val` - Value in ° (degrees)
    pub fn from_degrees(val: f64) -> Angle {
        return Angle(val.to_radians());
    }

    /// Gets the angle in rads (radians).
    pub fn as_radians(self) -> f64 {
        return self.0;
    }

    /// Gets the angle in ° (degrees).
    pub fn as_degrees(self) -> f64 {
        return self.0.to_degrees();
    }

    impl_num_methods!();
}
