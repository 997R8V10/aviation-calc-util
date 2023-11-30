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

use super::length::convert_feet_to_meters;
use super::length::convert_meters_to_feet;
use super::length::Length;

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

// Assignment Operator implementations
impl_assign_op_for_number!(Velocity, AddAssign, add_assign, +=);
impl_assign_op_for_number!(Velocity, SubAssign, sub_assign, -=);
impl_assign_op_for_number!(Velocity, MulAssign, mul_assign, *=);
impl_assign_op_for_number!(Velocity, DivAssign, div_assign, /=);
impl_assign_op_for_number!(Velocity, RemAssign, rem_assign, %=);

// Display
impl std::fmt::Display for Velocity {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        return write!(f, "{}m/s", self.0);
    }
}

impl Mul<std::time::Duration> for Velocity {
    type Output = Length;

    fn mul(self, rhs: std::time::Duration) -> Self::Output {
        return Length(self.0 * rhs.as_secs_f64());
    }
}

// Custom functions
impl Velocity {
    /// Creates a new Velocity.
    ///
    /// **Parameters:**
    /// - `val` - Value in m/s (meters per second)
    pub fn new(val: f64) -> Velocity {
        return Velocity(val);
    }

    /// Gets the velocity in m/s (meters per second).
    pub fn as_meters_per_second(self) -> f64 {
        return self.0;
    }

    /// Gets the velocity in kts (knots, nautical miles per hour)
    pub fn as_knots(self) -> f64 {
        return convert_mpers_to_kts(self.0);
    }

    /// Gets the velocity in ft/min (feet per minute)
    pub fn as_feet_per_minute(self) -> f64 {
        return convert_mpers_to_ftpermin(self.0);
    }

    /// Sets the velocity in m/s (meters per second).
    pub fn set_meters_per_second(&mut self, val: f64) {
        self.0 = val;
    }

    /// Sets the velocity in kts (knots, nautical miles per hour)
    pub fn set_knots(&mut self, val: f64) {
        self.0 = convert_kts_to_mpers(val);
    }

    /// Sets the velocity in ft/min (feet per minute)
    pub fn set_feet_per_minute(&mut self, val: f64) {
        self.0 = convert_ftpermin_to_mpers(val);
    }

    impl_num_methods!();
}

// Constants
/// Conversion factor from m/s (meters per second) to kts (knots, nautical miles per hour)
pub const CONV_FACTOR_MPERS_KTS: f64 = 1.943844;

// Conversion Functions
/// Convert m/s (meters per second) to kts (knots, nautical miles per hour)
pub fn convert_mpers_to_kts(mpers: f64) -> f64 {
    return mpers * CONV_FACTOR_MPERS_KTS;
}

/// Convert kts (knots, nautical miles per hour) to m/s (meters per second)
pub fn convert_kts_to_mpers(kts: f64) -> f64 {
    return kts / CONV_FACTOR_MPERS_KTS;
}

/// Convert m/s (meters per second) to ft/min (feet per minute)
pub fn convert_mpers_to_ftpermin(mpers: f64) -> f64 {
    return convert_meters_to_feet(60.0 * mpers);
}

/// Convert ft/min (feet per minute) to m/s (meters per second)
pub fn convert_ftpermin_to_mpers(ftpermin: f64) -> f64 {
    return convert_feet_to_meters(ftpermin) / 60.0;
}
