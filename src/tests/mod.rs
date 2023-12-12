use std::{env::temp_dir, thread, time::Duration};

use chrono::Utc;

use crate::{
    atmos::grib::GribTile,
    aviation,
    geo::{GeoPoint, Latitude, Longitude},
    units::{Angle, AngularVelocity, Length, Velocity, Unit},
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

#[test]
fn grib_test_1() {
    let point = GeoPoint::new(
        Latitude::new(Angle::new(0.0)),
        Longitude::new(Angle::new(0.0)),
        Length::new(0.0));

    let mut tile = GribTile::new(&point, &Utc::now(), &temp_dir().join("aviationcalc").join("gribtiles"));
    thread::sleep(Duration::from_millis(10000));

    let data_point = tile.closest_point(&point);

    assert!(data_point.is_some());

    println!("{}", data_point.unwrap());
}

#[test]
fn grib_test_2() {
    let point = GeoPoint::new(
        Latitude::new(Angle::new(35.0)),
        Longitude::new(Angle::new(-75.0)),
        Length::new(0.0));

    let mut tile = GribTile::new(&point, &Utc::now(), &temp_dir().join("aviationcalc").join("gribtiles"));

    let data_point = tile.closest_point(&point);

    assert!(data_point.is_none());
}