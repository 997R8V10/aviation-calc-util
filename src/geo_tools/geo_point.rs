use std::f64::consts::PI;

use crate::math_tools::math_util;

use super::geo_util::{self, GeoTrait};

#[derive(Clone, Copy, Default)]
pub struct GeoPoint {
    lat: f64,
    lon: f64,
    alt: f64,
}

impl GeoPoint {
    pub fn new(lat: f64, lon: f64, alt: f64) -> GeoPoint {
        GeoPoint {
            lat: f64::min(f64::max(lat, -90.0), 90.0),
            lon: geo_util::normalize_longitude(lon),
            alt: alt,
        }
    }

    pub fn move_by_m(&mut self, bearing: f64, distance: f64) {
        let R = geo_util::EARTH_RADIUS_M + math_util::convert_feet_to_meters(self.alt);
        let bearing_rads = bearing.normalize_heading().to_radians();
        let lat1 = self.lat.to_radians();
        let lon1 = self.lon.to_radians();

        let lat2 = f64::asin(
            f64::sin(lat1) * f64::cos(distance / R)
                + f64::cos(lat1) * f64::sin(distance / R) * f64::cos(bearing_rads),
        );

        let lon2 = lon1
            + f64::atan2(
                f64::sin(bearing_rads) * f64::sin(distance / R) * f64::cos(lat1),
                f64::cos(distance / R) - f64::sin(lat1) * f64::sin(lat2),
            );

        self.lat = lat2.to_degrees();
        self.lon = lon2.to_degrees();
    }

    pub fn move_by_NMi(&mut self, bearing: f64, distance: f64) {
        return self.move_by_m(
            bearing,
            math_util::convert_nautical_miles_to_meters(distance),
        );
    }

    pub fn flat_distance_m(&self, other: &GeoPoint) -> f64 {
        let phi1 = self.lat.to_radians();
        let phi2 = other.lat.to_radians();
        let delta_phi = (other.lat - self.lat).to_radians();
        let delta_lambda = (other.lon - self.lon).to_radians();

        let a = f64::sin(delta_phi / 2.0).powi(2)
            + f64::cos(phi1) * f64::cos(phi2) * f64::sin(delta_lambda / 2.0).powi(2);

        // Check for opposite end of planet
        if (1.0 - a <= 0.0) {
            // Return half circumference
            return geo_util::EARTH_RADIUS_M * PI;
        }

        let c = 2.0 * f64::atan2(f64::sqrt(a), f64::sqrt(1.0 - a));

        let d = geo_util::EARTH_RADIUS_M * c;

        return d;
    }
}
