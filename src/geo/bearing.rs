use std::f64::consts::PI;
use std::ops::Add;
use std::ops::AddAssign;
use std::ops::Neg;
use std::ops::Sub;
use std::ops::SubAssign;

use crate::units::angle::Angle;

/// Represents a bearing from 0 to 2pi rads (radians) or 0 to 360° (degrees).
#[derive(Clone, Copy, Default, PartialEq, Debug)]
pub struct Bearing(Angle);

impl Neg for Bearing {
    type Output = Bearing;

    fn neg(self) -> Self::Output {
        return Bearing((self.0 + PI) % (2.0 * PI));
    }
}

impl Add<Angle> for Bearing {
    type Output = Bearing;

    fn add(self, rhs: Angle) -> Self::Output {
        return Bearing(normalize_bearing(self.0 + rhs));
    }
}

impl AddAssign<Angle> for Bearing {
    fn add_assign(&mut self, rhs: Angle) {
        self.0 = normalize_bearing(self.0 + rhs);
    }
}

impl Sub for Bearing {
    type Output = Angle;

    fn sub(self, rhs: Self) -> Self::Output {
        return calculate_bearing_delta(self, rhs);
    }
}

impl Sub<Angle> for Bearing {
    type Output = Bearing;

    fn sub(self, rhs: Angle) -> Self::Output {
        return Bearing(normalize_bearing(self.0 - rhs));
    }
}

impl SubAssign<Angle> for Bearing {
    fn sub_assign(&mut self, rhs: Angle) {
        self.0 = normalize_bearing(self.0 - rhs);
    }
}

// Display
impl std::fmt::Display for Bearing {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        return write!(f, "{}°", self.0.as_degrees());
    }
}

// Custom functions
impl Bearing {
    /// Creates a new bearing.
    pub fn new(val: Angle) -> Bearing {
        return Bearing(normalize_bearing(val));
    }

    /// Create a new bearing from rads (radians).
    pub fn from_radians(val: f64) -> Bearing {
        return Self::new(Angle::new(val));
    }

    /// Create a new bearing from ° (degrees).
    pub fn from_degrees(val: f64) -> Bearing {
        return Self::new(Angle::from_degrees(val));
    }

    /// Gets the bearing as an Angle
    pub fn as_angle(self) -> Angle {
        return self.0;
    }

    /// Gets the bearing in rads (radians).
    pub fn as_radians(self) -> f64 {
        return self.0.as_radians();
    }

    /// Gets the bearing in ° (degrees).
    pub fn as_degrees(self) -> f64 {
        return self.0.as_degrees();
    }
}

/// Normalizes angle between 0 to 2pi rads (radians) or 0 to 360° (degrees).
///
/// **Parameters:**
/// - `angle` - Input Angle
///
/// **Returns:** \
/// Normalized Angle
///
/// ## Example
///
/// ```
/// use aviation_calc_util::geo::bearing;
/// use aviation_calc_util::units::angle::Angle;
///
/// let expected = Angle::from_degrees(10.0);
/// let calculated = bearing::normalize_bearing(Angle::from_degrees(370.0));
/// 
/// let abs_diff = f64::abs(expected.as_radians() - calculated.as_radians());
/// 
/// assert!(abs_diff < 1.0);
/// ```
pub fn normalize_bearing(angle: Angle) -> Angle {
    return (angle + (2.0 * PI)) % (2.0 * PI);
}

/// Calculate the amount of turn for the shortest turn between two bearings.
///
/// Positive indicates right turn, Negative indicates left turn.
///
/// # Examples
///
/// ```
/// use aviation_calc_util::geo::bearing::calculate_bearing_delta;
/// use aviation_calc_util::{geo::bearing::Bearing, units::angle::Angle};
/// 
/// let expected = Angle::from_degrees(-20.0);
/// let calculated = calculate_bearing_delta(Bearing::from_degrees(10.0), Bearing::from_degrees(350.0));
/// 
/// let abs_diff = f64::abs(expected.as_radians() - calculated.as_radians());
///
/// assert!(abs_diff < 1.0);
/// ```
pub fn calculate_bearing_delta(bearing_1: Bearing, bearing_2: Bearing) -> Angle {
    // Either distance or 360 - distance
    let raw_delta = bearing_2.as_radians() - bearing_1.as_radians();
    let phi = raw_delta.abs() % (2.0 * PI);

    let distance = if phi > PI { (2.0 * PI) - phi } else { phi };

    // Figure out if left turn or right turn
    let mut sign = 1.0;
    let raw_delta_opposite = bearing_1.as_radians() - bearing_2.as_radians();
    if (raw_delta_opposite >= 0.0 && raw_delta_opposite <= PI) || (raw_delta_opposite <= -PI && raw_delta_opposite >= -2.0 * PI) {
        sign = -1.0;
    }

    return Angle::new(distance * sign);
}

/// Calculates the amount of turn for a turn from one bearing to another.
///
/// Positive indicates right turn, Negative indicates left turn.
pub fn calculate_bearing_delta_with_dir(bearing_1: Bearing, bearing_2: Bearing, is_right_turn: bool) -> Angle {
    let mut delta = if is_right_turn {
        bearing_2.as_radians() - bearing_1.as_radians()
    } else {
        bearing_1.as_radians() - bearing_2.as_radians()
    };

    // If it passes through 360, normalize it
    if delta < 0.0 {
        delta += 2.0 * PI;
    }

    return Angle::new(delta);
}

/// Calculates the end bearing given start bearing and turn amount
/// 
/// A negative bearing_delta indicates a left turn.
pub fn calculate_end_bearing(start_bearing: Bearing, bearing_delta: Angle) -> Bearing {
    return start_bearing + bearing_delta;
}