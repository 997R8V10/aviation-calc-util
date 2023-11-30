#![allow(unused_parens)]

pub mod macros;
pub mod math_tools;
pub mod geo_tools;

#[allow(non_snake_case)]
pub mod units;

pub fn add(left: usize, right: usize) -> usize {
    left + right
}

#[cfg(test)]
mod tests {
    use crate::{geo_tools::geo_util, units::{velocity::Velocity, angle::Angle, angular_velocity::AngularVelocity}};

    #[test]
    fn it_works() {
        let result = geo_util::calculate_max_bank_angle(Velocity::from_knots(250.0), Angle::from_degrees(25.0), AngularVelocity::from_degrees_per_second(3.0));
        assert_eq!(result, Angle::from_degrees(25.0));
    }
}
