use std::path::PathBuf;

use chrono::NaiveDate;

use crate::{geo::{Bearing, GeoPoint, Latitude, Longitude}, magnetic::MagneticModel, units::Length};



#[test]
fn magnetic_to_true_1() {
    let filename = PathBuf::from(env!("CARGO_MANIFEST_DIR")).join("src").join("tests").join("magnetic").join("WMM.COF");

    println!("{}", filename.display());

    let mag_tile_model = MagneticModel::from_file(&filename.into_os_string().into_string().unwrap()).unwrap();

    let point = GeoPoint::from_degs_and_ft(54_f64, -6_f64, 5000_f64);
    let date = NaiveDate::from_ymd_opt(2024, 01 as u32, 09 as u32).unwrap();

    let field = mag_tile_model.calculate_field(&point, &date);

    println!("{:?}", field.field_elements);

    assert!((field.field_elements.x - 18_334.5_f64).abs() <= 1.0);
    assert!((field.field_elements.y - -515.0_f64).abs() <= 1.0);
    assert!((field.field_elements.z - 46_054.7_f64).abs() <= 1.0);
    assert!((field.field_elements.h - 18_341.4_f64).abs() <= 1.0);
    assert!((field.field_elements.f - 49_572.6_f64).abs() <= 1.0);
}

#[test]
fn test_move_by_3(){
    let filename = PathBuf::from(env!("CARGO_MANIFEST_DIR")).join("src").join("tests").join("magnetic").join("WMM.COF");
    let mag_tile_model = MagneticModel::from_file(&filename.into_os_string().into_string().unwrap()).unwrap();

    let date = NaiveDate::from_ymd_opt(2022, 11 as u32, 11 as u32).unwrap();

    let jmllr = GeoPoint::new(
        Latitude::from_deg_min_sec(39, 34, 0.4 * 60_f64),
        Longitude::from_deg_min_sec(-119, 45, 0.7 * 60_f64),
        Length::default()
    );

    let field = mag_tile_model.calculate_field(&jmllr, &date);    

    let mut zitip = jmllr.clone();
    zitip.move_by(field.magnetic_to_true(Bearing::from_degrees(5_f64)), Length::from_nautical_miles(31.3));

    println!("{:?} {:?}", zitip.lat.as_deg_min_sec(), zitip.lon.as_deg_min_sec());
}