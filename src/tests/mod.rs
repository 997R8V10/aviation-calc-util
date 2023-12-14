use std::{env::temp_dir, thread, time::Duration};

use chrono::Utc;

use crate::{
    atmos::{self, convert_tas_to_ias, grib::GribTile},
    aviation::{self, calculate_arc_course_intercept},
    geo::{Bearing, GeoPoint, Latitude, Longitude},
    units::{Angle, AngularVelocity, Length, Pressure, Temperature, Unit, Velocity},
};

#[test]
fn true_airspeed_1() {
    let online_tas = Velocity::from_knots(483.0);

    let h = Length::from_feet(35_000.0);
    let t = Temperature::from_celsius(-39.0);
    let p = atmos::calculate_pressure_at_alt(h, Length::default(), atmos::ISA_STD_PRES, t);
    let cas = Velocity::from_knots(260.0);
    let qc = atmos::calculate_impact_pressure_at_cas(cas);
    let m = atmos::calculate_mach_number(qc, p);
    let eas = atmos::calculate_eas(m, p);
    let atmos_tas = atmos::convert_mach_to_tas(m, t);
    let atmos_tas2 = atmos::convert_ias_to_tas(cas, p, h, h, t);

    println!("\tMach: \t\t{}", m);
    println!("\tEAS: \t\t{}kts", eas.as_knots());
    println!("\tOnline TAS: \t{}kts", online_tas.as_knots());
    println!("\tAtmos. TAS: \t{}kts", atmos_tas.as_knots());
    println!("\tAtmos. TAS: \t{}kts", atmos_tas2.0.as_knots());

    assert!((atmos_tas.as_knots() - online_tas.as_knots()).abs() <= 5.0);
}

#[test]
fn convert_tas_to_ias_1() {
    let (ias, _mach) = convert_tas_to_ias(
        Velocity::from_knots(421.0),
        Pressure::from_hectopascals(250.0),
        Length::from_feet(35_269.0),
        Length::from_feet(34_873.0),
        Temperature::from_kelvin(221.0),
    );

    assert!((ias.as_knots() - 250.0) <= 1.0);
}

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
fn grib_test_1() {
    let point = GeoPoint::new(Latitude::new(Angle::new(0.0)), Longitude::new(Angle::new(0.0)), Length::new(0.0));

    let mut tile = GribTile::new(&point, &Utc::now(), &temp_dir().join("aviationcalc").join("gribtiles"));
    thread::sleep(Duration::from_millis(10000));

    let data_point = tile.closest_point(&point);

    assert!(data_point.is_some());

    println!("{}", data_point.unwrap());
}

#[test]
fn grib_test_2() {
    let point = GeoPoint::new(Latitude::new(Angle::new(35.0)), Longitude::new(Angle::new(-75.0)), Length::new(0.0));

    let mut tile = GribTile::new(&point, &Utc::now(), &temp_dir().join("aviationcalc").join("gribtiles"));

    let data_point = tile.closest_point(&point);

    assert!(data_point.is_none());
}

#[test]
fn aviation_arc_course_intercept_1() {
    let center = GeoPoint::new(Latitude::from_degrees(38.0), Longitude::from_degrees(-77.0), Length::new(0.0));
    let mut aircraft = GeoPoint::new(Latitude::from_degrees(38.0), Longitude::from_degrees(-77.0), Length::new(0.0));
    aircraft.move_by(Bearing::from_degrees(280.0), Length::from_meters(11000.0));

    let intercept_info = calculate_arc_course_intercept(
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
