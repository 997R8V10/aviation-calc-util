use std::ops::Add;
use std::ops::Div;
use std::ops::Mul;
use std::ops::Neg;
use std::ops::Rem;
use std::ops::Sub;

use crate::impl_one_part_op_for_number;
use crate::impl_two_part_op_for_number;

use super::unit::Unit;

/// Represents a pressure quantity.
#[derive(Clone, Copy, Default, PartialEq, PartialOrd, Debug)]
pub struct Pressure(pub(crate) f64);

// Operator implementations
impl_one_part_op_for_number!(Pressure, Neg, neg, -);
impl_two_part_op_for_number!(Pressure, Add, add, +);
impl_two_part_op_for_number!(Pressure, Sub, sub, -);
impl_two_part_op_for_number!(Pressure, Mul, mul, *);
impl_two_part_op_for_number!(Pressure, Div, div, /);
impl_two_part_op_for_number!(Pressure, Rem, rem, %);

// Display
impl std::fmt::Display for Pressure {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        return write!(f, "{}Pa", self.0);
    }
}

impl Unit for Pressure {
    /// Creates a new Pressure.
    ///
    /// **Parameters:**
    /// - `val` - Value in Pa (pascals)
    fn new(val: f64) -> Pressure {
        return Pressure(val);
    }

    /// Gets the pressure in Pa (pascals).
    fn value(&self) -> f64 {
        return self.0;
    }
}

// Custom functions
impl Pressure {
    /// Creates a new Pressure.
    ///
    /// **Parameters:**
    /// - `val` - Value in Pa (pascals)
    pub fn from_pascals(val: f64) -> Pressure {
        return Pressure(val);
    }

    /// Creates a new Pressure.
    ///
    /// **Parameters:**
    /// - `hectopascals` - Value in hPa (hectopascals).
    pub fn from_hectopascals(hectopascals: f64) -> Pressure {
        return Pressure(hectopascals * 100.0);
    }

    /// Creates a new Pressure.
    ///
    /// **Parameters:**
    /// - `inches_of_mercury` - Value in inHg (inches of mercury).
    pub fn from_inches_of_mercury(inches_of_mercury: f64) -> Pressure {
        return Pressure(Self::convert_inHg_to_Pa(inches_of_mercury));
    }

    /// Gets the pressure in Pa (pascals).
    pub fn as_pascals(self) -> f64 {
        return self.0;
    }

    /// Gets the pressure in inHg (inches of mercury).
    pub fn as_inches_of_mercury(self) -> f64 {
        return Self::convert_Pa_to_inHg(self.0);
    }

    /// Gets the pressure in hPa (hectopascals).
    pub fn as_hectopascals(self) -> f64 {
        return self.0 / 100.0;
    }

    // Constants
    /// Conversion factor from Pa (pascals) to inHg (inches of mercury)
    pub const CONV_FACTOR_PA_INHG: f64 = 3386.0;

    // Unit Conversion Functions
    /// Convert from hPa (hectopascals) to inHg (inches of mercury)
    pub fn convert_hPa_to_inHg(hPa: f64) -> f64 {
        return hPa / (Self::CONV_FACTOR_PA_INHG / 100.0);
    }

    /// Convert from Pa (pascals) to inHg (inches of mercury)
    pub fn convert_Pa_to_inHg(Pa: f64) -> f64 {
        return Pa / Self::CONV_FACTOR_PA_INHG;
    }

    /// Convert from inHg (inches of mercury) to hPa (hectopascals)
    pub fn convert_inHg_to_hPa(inHg: f64) -> f64 {
        return inHg * (Self::CONV_FACTOR_PA_INHG / 100.0);
    }

    /// Convert from inHg (inches of mercury) to Pa (pascals)
    pub fn convert_inHg_to_Pa(inHg: f64) -> f64 {
        return inHg * Self::CONV_FACTOR_PA_INHG;
    }
}
