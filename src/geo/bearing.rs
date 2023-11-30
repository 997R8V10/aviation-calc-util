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

impl Add for Bearing {
    type Output = Bearing;

    fn add(self, rhs: Self) -> Self::Output {
        return Bearing(normalize_bearing(self.0 + rhs.0));
    }
}

impl Add<Angle> for Bearing {
    type Output = Bearing;

    fn add(self, rhs: Angle) -> Self::Output {
        return Bearing(normalize_bearing(self.0 + rhs));
    }
}

impl AddAssign for Bearing {
    fn add_assign(&mut self, rhs: Self) {
        self.0 = normalize_bearing(self.0 + rhs.0);
    }
}

impl AddAssign<Angle> for Bearing {
    fn add_assign(&mut self, rhs: Angle) {
        self.0 = normalize_bearing(self.0 + rhs);
    }
}

impl Sub for Bearing {
    type Output = Bearing;

    fn sub(self, rhs: Self) -> Self::Output {
        return Bearing(normalize_bearing(self.0 - rhs.0));
    }
}

impl Sub<Angle> for Bearing {
    type Output = Bearing;

    fn sub(self, rhs: Angle) -> Self::Output {
        return Bearing(normalize_bearing(self.0 - rhs));
    }
}

impl SubAssign for Bearing {
    fn sub_assign(&mut self, rhs: Self) {
        self.0 = normalize_bearing(self.0 - rhs.0);
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
/// use aviation_calc_util::geo_tools::bearing;
/// use aviation_calc_util::units::angle::Angle;
///
/// assert_eq!(bearing::normalize_bearing(Angle::from_degrees(370.0)), Angle::from_degrees(10.0));
/// ```
pub fn normalize_bearing(angle: Angle) -> Angle {
    return (angle + (2.0 * PI)) % (2.0 * PI);
}