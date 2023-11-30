use crate::units::{length::Length, angular_velocity::AngularVelocity, velocity::Velocity, angle::Angle};

// Constants
pub const EARTH_RADIUS: Length = Length(6371e3);
pub const GRAVITY_ACCEL: f64 = 9.81;

pub fn calculate_max_bank_angle(ground_speed: Velocity, bank_limit: Angle, turn_rate: AngularVelocity) -> Angle {
    // bank angle = atan(rate of turn * v / g)
    let bank_angle = Angle(f64::atan2(turn_rate.as_radians_per_second() * ground_speed.as_meters_per_second(), GRAVITY_ACCEL));

    return if bank_angle > bank_limit {bank_limit} else {bank_angle};
}

