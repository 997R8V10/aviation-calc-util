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

/// Represents a temperature quantity.
#[derive(Clone, Copy, Default, PartialEq, PartialOrd, Debug)]
pub struct Temperature(pub(crate) f64);

// Operator implementations
impl_one_part_op_for_number!(Temperature, Neg, neg, -);
impl_two_part_op_for_number!(Temperature, Add, add, +);
impl_two_part_op_for_number!(Temperature, Sub, sub, -);
impl_two_part_op_for_number!(Temperature, Mul, mul, *);
impl_two_part_op_for_number!(Temperature, Div, div, /);
impl_two_part_op_for_number!(Temperature, Rem, rem, %);

// Assignment Operator implementations
impl_assign_op_for_number!(Temperature, AddAssign, add_assign, +=);
impl_assign_op_for_number!(Temperature, SubAssign, sub_assign, -=);
impl_assign_op_for_number!(Temperature, MulAssign, mul_assign, *=);
impl_assign_op_for_number!(Temperature, DivAssign, div_assign, /=);
impl_assign_op_for_number!(Temperature, RemAssign, rem_assign, %=);

// Display
impl std::fmt::Display for Temperature {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        return write!(f, "{}K", self.0);
    }
}

// Custom functions
impl Temperature {
    /// Creates a new Temperature.
    ///
    /// **Parameters:**
    /// - `val` - Value in K (kelvin)
    pub fn new(val: f64) -> Temperature {
        return Temperature(val);
    }

    /// Creates a new Temperature.
    ///
    /// **Parameters:**
    /// - `val` - Value in °C (degrees celsius).
    pub fn from_celsius(val: f64) -> Temperature {
        return Temperature(convert_celsius_to_kelvin(val));
    }

    /// Gets the temperature in K (kelvin).
    pub fn as_kelvin(self) -> f64 {
        return self.0;
    }

    /// Gets the temperature in °C (degrees celsius).
    pub fn as_celsius(self) -> f64 {
        return convert_kelvin_to_celsius(self.0);
    }

    impl_num_methods!();
}

// Constants
/// Conversion factor from K (kelvin) to °C (degrees celsius)
pub const CONV_FACTOR_KELVIN_C: f64 = 273.15;

// Unit conversion functions
/// Convert K (kelvin) to °C (degrees celsius)
pub fn convert_kelvin_to_celsius(kelvin: f64) -> f64 {
    return kelvin - CONV_FACTOR_KELVIN_C;
}

/// Convert °C (degrees celsius) to K (kelvin)
pub fn convert_celsius_to_kelvin(celsius: f64) -> f64 {
    return celsius + CONV_FACTOR_KELVIN_C;
}
