use std::{env::temp_dir, time::Duration, thread};

use chrono::Utc;

use crate::{geo::{GeoPoint, Latitude, Longitude}, units::{Angle, Length, Unit}, atmos::grib::GribTile};

#[test]
fn grib_test_1() {
    let point = GeoPoint::new(Latitude::new(Angle::new(0.0)), Longitude::new(Angle::new(0.0)), Length::new(0.0));

    let mut tile = GribTile::new(&point, &Utc::now(), &temp_dir().join("aviationcalc").join("gribtiles"));

    let mut i = 0;

    let mut data_point = None;
    while (i < 30 && data_point.is_none()){
        data_point = tile.closest_point(&point);
        thread::sleep(Duration::from_millis(1000));
        i += 1;
    }
    
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