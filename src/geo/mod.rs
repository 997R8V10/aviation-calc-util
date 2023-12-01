use crate::units::length::Length;

pub mod geo_point;
pub mod bearing;
pub mod latitude;
pub mod longitude;

// Constants
pub const EARTH_RADIUS: Length = Length(6371e3);
pub const GRAVITY_ACCEL: f64 = 9.81;