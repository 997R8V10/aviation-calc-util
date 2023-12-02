use crate::{
    aviation,
    units::{angle::Angle, angular_velocity::AngularVelocity, velocity::Velocity},
};

#[test]
fn it_works() {
    let result = aviation::calculate_max_bank_angle(
        Velocity::from_knots(250.0),
        Angle::from_degrees(25.0),
        AngularVelocity::from_degrees_per_second(3.0),
    );
    assert_eq!(result, Angle::from_degrees(25.0));
}
