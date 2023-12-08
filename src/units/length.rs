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

use super::velocity::Velocity;

/// Represents a length quantity.
#[derive(Clone, Copy, Default, PartialEq, PartialOrd, Debug)]
pub struct Length(pub(crate) f64);

// Operator implementations
impl_one_part_op_for_number!(Length, Neg, neg, -);
impl_two_part_op_for_number!(Length, Add, add, +);
impl_two_part_op_for_number!(Length, Sub, sub, -);
impl_two_part_op_for_number!(Length, Mul, mul, *);
impl_two_part_op_for_number!(Length, Div, div, /);
impl_two_part_op_for_number!(Length, Rem, rem, %);

// Assignment Operator implementations
impl_assign_op_for_number!(Length, AddAssign, add_assign, +=);
impl_assign_op_for_number!(Length, SubAssign, sub_assign, -=);
impl_assign_op_for_number!(Length, MulAssign, mul_assign, *=);
impl_assign_op_for_number!(Length, DivAssign, div_assign, /=);
impl_assign_op_for_number!(Length, RemAssign, rem_assign, %=);

// Display
impl std::fmt::Display for Length {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        return write!(f, "{}m", self.0);
    }
}

impl Div<std::time::Duration> for Length {
    type Output = Velocity;

    fn div(self, rhs: std::time::Duration) -> Self::Output {
        return Velocity::new(self.0 / rhs.as_secs_f64());
    }
}

// Custom functions
impl Length {
    /// Creates a new Length.
    ///
    /// **Parameters:**
    /// - `val` - Value in m (meters)
    pub fn new(val: f64) -> Length {
        return Length(val);
    }

    /// Creates a new Length.
    ///
    /// **Parameters:**
    /// - `val_feet` - Value in ft (feet)
    pub fn from_feet(val_feet: f64) -> Length {
        return Length(convert_feet_to_meters(val_feet));
    }

    /// Creates a new Length.
    ///
    /// **Parameters:**
    /// - `val_nautical_miles` - Value in nmi (nautical miles)
    pub fn from_nautical_miles(val_nautical_miles: f64) -> Length {
        return Length(convert_nautical_miles_to_meters(val_nautical_miles));
    }

    /// Creates a new Length.
    ///
    /// **Parameters:**
    /// - `val_statute_miles` - Value in mi (statute miles)
    pub fn from_statute_miles(val_statute_miles: f64) -> Length {
        return Length(convert_statute_miles_to_meters(val_statute_miles));
    }

    /// Gets the length in m (meters).
    pub fn as_meters(self) -> f64 {
        return self.0;
    }

    /// Gets the length in ft (feet).
    pub fn as_feet(self) -> f64 {
        return convert_meters_to_feet(self.0);
    }

    /// Gets the length in mi (statute miles).
    pub fn as_statute_miles(self) -> f64 {
        return convert_meters_to_statute_miles(self.0);
    }

    /// Gets the length in nmi (nautical miles).
    pub fn as_nautical_miles(self) -> f64 {
        return convert_meters_to_nautical_miles(self.0);
    }

    impl_num_methods!();
}

// Constants
/// Conversion factor from m (meters) to ft (feet)
pub const CONV_FACTOR_M_FT: f64 = 3.28084;
/// Conversion factor from nmi (nautical miles) to m (meters)
pub const CONV_FACTOR_NMI_M: f64 = 1852.0;
/// Conversion factor from mi (statute miles) to m (meters)
pub const CONV_FACTOR_MI_M: f64 = 1609.344;

// Unit Conversion Functions
/// Convert m (meters) to ft (feet)
pub fn convert_meters_to_feet(meters: f64) -> f64 {
    return meters * CONV_FACTOR_M_FT;
}

/// Convert m (meters) to nmi (nautical miles)
pub fn convert_meters_to_nautical_miles(meters: f64) -> f64 {
    return meters / CONV_FACTOR_NMI_M;
}

/// Convert m (meters) to mi (statute miles)
pub fn convert_meters_to_statute_miles(meters: f64) -> f64 {
    return meters / CONV_FACTOR_MI_M;
}

/// Convert ft (feet) to m (meters)
pub fn convert_feet_to_meters(feet: f64) -> f64 {
    return feet / CONV_FACTOR_M_FT;
}

/// Convert nmi (nautical miles) to m (meters)
pub fn convert_nautical_miles_to_meters(nmi: f64) -> f64 {
    return nmi * CONV_FACTOR_NMI_M;
}

/// Convert mi (statute miles) to m (meters)
pub fn convert_statute_miles_to_meters(mi: f64) -> f64 {
    return mi * CONV_FACTOR_MI_M;
}
