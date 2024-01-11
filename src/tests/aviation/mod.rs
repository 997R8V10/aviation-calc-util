use crate::{
    aviation,
    geo::{Bearing, GeoPoint, Latitude, Longitude},
    units::{Angle, AngularVelocity, Length, Unit, Velocity},
};

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
fn aviation_calculate_chord_for_turn_1() {
    let start_bearing = Bearing::from_degrees(120_f64);
    let turn_amt = Angle::from_degrees(5.0_f64);
    let radius_of_turn = Length::from_meters(9000_f64);

    let (calculated_bearing, calculated_length) = aviation::calculate_chord_for_turn(start_bearing, turn_amt, radius_of_turn);
    let (expected_bearing, expected_length) = (Bearing::from_degrees(122.5_f64), Length::from_meters(785.148972576));

    assert!((calculated_bearing - expected_bearing).as_degrees().abs() <= 0.01);
    assert!((calculated_length - expected_length).as_meters().abs() <= 1.0);
}

#[test]
fn aviation_calculate_chord_for_turn_2() {
    let start_bearing = Bearing::from_degrees(120_f64);
    let turn_amt = Angle::from_degrees(-5.0_f64);
    let radius_of_turn = Length::from_meters(9000_f64);

    let (calculated_bearing, calculated_length) = aviation::calculate_chord_for_turn(start_bearing, turn_amt, radius_of_turn);
    let (expected_bearing, expected_length) = (Bearing::from_degrees(117.5_f64), Length::from_meters(785.148972576));

    assert!((calculated_bearing - expected_bearing).as_degrees().abs() <= 0.01);
    assert!((calculated_length - expected_length).as_meters().abs() <= 1.0);
}

#[test]
fn aviation_calculate_turn_lead_dist_1() {
    let waypoint = GeoPoint::from_degs_and_ft(1_f64, 0_f64, 0_f64);
    let aircraft = GeoPoint::from_degs_and_ft(0.5_f64, 0.5_f64, 35_000_f64);
    let aircraft_course = Bearing::from_degrees(270_f64);
    let waypoint_course = Bearing::from_degrees(360_f64);

    let calculated = aviation::calculate_turn_lead_distance(
        &aircraft,
        &waypoint,
        aircraft_course,
        Velocity::from_knots(400_f64),
        waypoint_course,
        Bearing::from_degrees(335_f64),
        Velocity::from_knots(50_f64),
        Angle::from_degrees(25_f64),
        AngularVelocity::from_degrees_per_second(3_f64),
    );

    if (calculated.is_some()){
        println!("{:?}", calculated);
    }
}

#[test]
fn aviation_calculate_turn_lead_dist_2() {
    let waypoint = GeoPoint::from_degs_and_ft(54.45237778_f64, -5.3348194400000422_f64, 0_f64);
    let aircraft = GeoPoint::from_degs_and_ft(54.3527834_f64, -5.07700259999998_f64, 10_000_f64);
    let aircraft_course = Bearing::from_degrees(32.111653320461478_f64);
    let waypoint_course = Bearing::from_degrees(313.98221920547149_f64);
    let true_airspeed = Velocity::from_knots(290.58338915806166);
    let wind_dir = Bearing::from_degrees(0.0);
    let wind_spd = Velocity::from_knots(0.0);

    let calculated = aviation::calculate_turn_lead_distance(
        &aircraft,
        &waypoint,
        aircraft_course,
        true_airspeed,
        waypoint_course,
        wind_dir,
        wind_spd,
        Angle::from_degrees(25_f64),
        AngularVelocity::from_degrees_per_second(3_f64),
    );

    if (calculated.is_some()){
        println!("{:?}", calculated);
    }
}
