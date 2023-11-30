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

// Assignment Operator implementations
impl_assign_op_for_number!(Pressure, AddAssign, add_assign, +=);
impl_assign_op_for_number!(Pressure, SubAssign, sub_assign, -=);
impl_assign_op_for_number!(Pressure, MulAssign, mul_assign, *=);
impl_assign_op_for_number!(Pressure, DivAssign, div_assign, /=);
impl_assign_op_for_number!(Pressure, RemAssign, rem_assign, %=);

// Display
impl std::fmt::Display for Pressure {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        return write!(f, "{}Pa", self.0);
    }
}

// Custom functions
impl Pressure {
    /// Creates a new Pressure.
    ///
    /// **Parameters:**
    /// - `val` - Value in Pa (pascals)
    pub fn new(val: f64) -> Pressure {
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
        return Pressure(convert_inHg_to_Pa(inches_of_mercury));
    }

    /// Gets the pressure in Pa (pascals).
    pub fn as_pascals(self) -> f64 {
        return self.0;
    }

    /// Gets the pressure in inHg (inches of mercury).
    pub fn as_inches_of_mercury(self) -> f64 {
        return convert_Pa_to_inHg(self.0);
    }

    /// Gets the pressure in hPa (hectopascals).
    pub fn as_hectopascals(self) -> f64 {
        return self.0 / 100.0;
    }

    impl_num_methods!();
}

// Constants
/// Conversion factor from Pa (pascals) to inHg (inches of mercury)
pub const CONV_FACTOR_PA_INHG: f64 = 3386.0;

// Unit Conversion Functions
/// Convert from hPa (hectopascals) to inHg (inches of mercury)
pub fn convert_hPa_to_inHg(hPa: f64) -> f64 {
    return hPa / (CONV_FACTOR_PA_INHG / 100.0);
}

/// Convert from Pa (pascals) to inHg (inches of mercury)
pub fn convert_Pa_to_inHg(Pa: f64) -> f64 {
    return Pa / CONV_FACTOR_PA_INHG;
}

/// Convert from inHg (inches of mercury) to hPa (hectopascals)
pub fn convert_inHg_to_hPa(inHg: f64) -> f64 {
    return inHg * (CONV_FACTOR_PA_INHG / 100.0);
}

/// Convert from inHg (inches of mercury) to Pa (pascals)
pub fn convert_inHg_to_Pa(inHg: f64) -> f64 {
    return inHg * CONV_FACTOR_PA_INHG;
}
