use crate::{aviation, units::{Velocity, Angle, AngularVelocity, Length, Unit}, geo::{GeoPoint, Latitude, Longitude, Bearing}};

#[test]
fn calculate_max_bank_angle_1() {
    let result = aviation::calculate_max_bank_angle(
        Velocity::from_knots(250.0),
        Angle::from_degrees(25.0),
        AngularVelocity::from_degrees_per_second(3.0),
    );

    assert_eq!(result, Angle::from_degrees(25.0));
}

#[test]
fn aviation_arc_course_intercept_1() {
    let center = GeoPoint::new(Latitude::from_degrees(38.0), Longitude::from_degrees(-77.0), Length::new(0.0));
    let mut aircraft = GeoPoint::new(Latitude::from_degrees(38.0), Longitude::from_degrees(-77.0), Length::new(0.0));
    aircraft.move_by(Bearing::from_degrees(280.0), Length::from_meters(11000.0));

    let intercept_info = aviation::calculate_arc_course_intercept(
        &aircraft,
        &center,
        Bearing::from_degrees(250.0),
        Bearing::from_degrees(30.0),
        Length::from_meters(10000.0),
        true,
    );

    assert!((intercept_info.cross_track_error.as_meters() - -1000.0).abs() <= 1.0);
    assert!((intercept_info.required_course.as_degrees() - 10.0).abs() <= 0.1);
    assert!((intercept_info.along_track_distance.as_meters() - 19198.62).abs() <= 1.0);
}

#[test]
fn aviation_calculate_chord_for_turn_1(){
    let start_bearing = Bearing::from_degrees(120_f64);
    let turn_amt = Angle::from_degrees(5.0_f64);
    let radius_of_turn = Length::from_meters(9000_f64);

    let (calculated_bearing, calculated_length) = aviation::calculate_chord_for_turn(start_bearing, turn_amt, radius_of_turn);
    let (expected_bearing, expected_length) = (Bearing::from_degrees(122.5_f64), Length::from_meters(785.148972576));

    assert!((calculated_bearing - expected_bearing).as_degrees().abs() <= 0.01);
    assert!((calculated_length - expected_length).as_meters().abs() <= 1.0);
}

#[test]
fn aviation_calculate_chord_for_turn_2(){
    let start_bearing = Bearing::from_degrees(120_f64);
    let turn_amt = Angle::from_degrees(-5.0_f64);
    let radius_of_turn = Length::from_meters(9000_f64);

    let (calculated_bearing, calculated_length) = aviation::calculate_chord_for_turn(start_bearing, turn_amt, radius_of_turn);
    let (expected_bearing, expected_length) = (Bearing::from_degrees(117.5_f64), Length::from_meters(785.148972576));

    assert!((calculated_bearing - expected_bearing).as_degrees().abs() <= 0.01);
    assert!((calculated_length - expected_length).as_meters().abs() <= 1.0);
}