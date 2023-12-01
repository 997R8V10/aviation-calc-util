#![allow(unused_parens)]

pub mod macros;
pub mod math;
pub mod geo;
pub mod aviation;
pub mod atmos;

#[allow(non_snake_case)]
pub mod units;

pub fn add(left: usize, right: usize) -> usize {
    left + right
}

#[cfg(test)]
mod tests {
    use crate::{units::{velocity::Velocity, angle::Angle, angular_velocity::AngularVelocity}, aviation};

    #[test]
    fn it_works() {
        let result = aviation::calculate_max_bank_angle(Velocity::from_knots(250.0), Angle::from_degrees(25.0), AngularVelocity::from_degrees_per_second(3.0));
        assert_eq!(result, Angle::from_degrees(25.0));
    }
}
