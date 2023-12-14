use std::ops::Add;
use std::ops::Div;
use std::ops::Mul;
use std::ops::Neg;
use std::ops::Rem;
use std::ops::Sub;

use crate::impl_one_part_op_for_number;
use crate::impl_two_part_op_for_number;

use super::unit::Unit;

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

// Display
impl std::fmt::Display for Temperature {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        return write!(f, "{}K", self.0);
    }
}

impl Unit for Temperature {
    /// Creates a new Temperature.
    ///
    /// **Parameters:**
    /// - `val` - Value in K (kelvin)
    fn new(val: f64) -> Temperature {
        return Temperature(val);
    }

    /// Gets the temperature in K (kelvin).
    fn value(&self) -> f64 {
        return self.0;
    }
}

impl From<f64> for Temperature {
    fn from(value: f64) -> Self {
        return Self::new(value);
    }
}

impl From<Temperature> for f64 {
    fn from(value: Temperature) -> Self {
        return value.value();
    }
}

// Custom functions
impl Temperature {
    /// Creates a new Temperature.
    ///
    /// **Parameters:**
    /// - `val` - Value in K (kelvin)
    pub fn from_kelvin(val: f64) -> Temperature {
        return Temperature(val);
    }

    /// Creates a new Temperature.
    ///
    /// **Parameters:**
    /// - `val` - Value in °C (degrees celsius).
    pub fn from_celsius(val: f64) -> Temperature {
        return Temperature(Self::convert_celsius_to_kelvin(val));
    }

    /// Gets the temperature in K (kelvin).
    pub fn as_kelvin(&self) -> f64 {
        return self.0;
    }

    /// Gets the temperature in °C (degrees celsius).
    pub fn as_celsius(&self) -> f64 {
        return Self::convert_kelvin_to_celsius(self.0);
    }

    // Constants
    /// Conversion factor from K (kelvin) to °C (degrees celsius)
    pub const CONV_FACTOR_KELVIN_C: f64 = 273.15;
    
    // Unit conversion functions
    /// Convert K (kelvin) to °C (degrees celsius)
    pub fn convert_kelvin_to_celsius(kelvin: f64) -> f64 {
        return kelvin - Self::CONV_FACTOR_KELVIN_C;
    }
    
    /// Convert °C (degrees celsius) to K (kelvin)
    pub fn convert_celsius_to_kelvin(celsius: f64) -> f64 {
        return celsius + Self::CONV_FACTOR_KELVIN_C;
    }
}
