use crate::{geo::{Bearing, GeoPoint}, units::Length};


#[test]
fn test_move_by_1(){
    let mut point = GeoPoint::from_degs_and_ft(0_f64, 0_f64, 0_f64);

    // Move by 60NMI
    point.move_by(Bearing::from_degrees(360_f64), Length::from_meters(111_200_f64));

    let expected_point = GeoPoint::from_degs_and_ft(1_f64, 0_f64, 0_f64);

    let distance = point.flat_distance(&expected_point).as_meters();
    
    assert!(distance <= 10.0);
}

#[test]
fn test_move_by_2(){
    let mut point = GeoPoint::from_degs_and_ft(0_f64, 0_f64, 0_f64);

    // Move by 60NMI
    point.move_by(Bearing::from_degrees(45_f64), Length::from_meters(157.3_f64));

    let expected_point = GeoPoint::from_degs_and_ft(0.001, 0.001, 0_f64);

    let distance = point.flat_distance(&expected_point).as_meters();
    
    assert!(distance <= 1.0);
}