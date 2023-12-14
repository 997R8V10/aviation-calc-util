use std::f64::consts::{FRAC_PI_2, PI};

use crate::units::{Length, Angle, Unit};

use super::{latitude::Latitude, longitude::Longitude, geo_point::GeoPoint};

pub trait GeoTile {
    fn bottom_lat(&self) -> Latitude;
    fn top_lat(&self) -> Latitude;
    fn left_lon(&self) -> Longitude;
    fn right_lon(&self) -> Longitude;

    fn center_point(&self) -> GeoPoint {
        let center_lat = Latitude::from_radians((self.bottom_lat().as_radians() + self.top_lat().as_radians()) / 2.0);
        let center_lon = Longitude::from_radians((self.left_lon().as_radians() + self.right_lon().as_radians()) / 2.0);

        return GeoPoint::new(center_lat, center_lon, Length::new(0.0));
    }

    fn is_point_in_tile(&self, point: &GeoPoint) -> bool {
        return point.lat >= self.bottom_lat() && point.lat <= self.top_lat() && point.lon >= self.left_lon() && point.lon <= self.right_lon();
    }
}

#[derive(Clone, Copy, Default, PartialEq, Debug)]
pub struct GeoTileBounds {
    bot_lat: Latitude,
    top_lat: Latitude,
    left_lon: Longitude,
    right_lon: Longitude
}

impl GeoTile for GeoTileBounds {
    fn bottom_lat(&self) -> Latitude {
        return self.bot_lat;
    }

    fn top_lat(&self) -> Latitude {
        return self.top_lat;
    }

    fn left_lon(&self) -> Longitude {
        return self.left_lon;
    }

    fn right_lon(&self) -> Longitude {
        return self.right_lon;
    }
}

impl GeoTileBounds {
    pub fn new(bot_lat: Latitude, mut top_lat: Latitude, left_lon: Longitude, mut right_lon: Longitude) -> GeoTileBounds {
        if (right_lon < left_lon){
            right_lon = left_lon;
        }

        if (top_lat < bot_lat){
            top_lat = bot_lat;
        }

        return GeoTileBounds { bot_lat, top_lat, left_lon, right_lon };
    }

    pub fn new_from_point(point: &GeoPoint, resolution: Angle) -> GeoTileBounds {
        let res = resolution.as_radians().abs();

        // Get indexes for the speciefied resolution and floor it
        let max_lat_index = (FRAC_PI_2 / res).floor() as i32;
        let max_lon_index = (PI / res).floor() as i32;
        let mut lat_index = (point.lat.as_radians() / res).floor() as i32;
        let mut lon_index = (point.lon.as_radians() / res).floor() as i32;

        // Check index
        if lat_index == max_lat_index {
            lat_index -= 1;
        }

        if lon_index == max_lon_index {
            lon_index -= 1;
        }

        return GeoTileBounds {
            bot_lat: Latitude::from_radians(lat_index as f64 * res),
            top_lat: Latitude::from_radians((lat_index + 1) as f64 * res),
            left_lon: Longitude::from_radians(lon_index as f64 * res),
            right_lon: Longitude::from_radians((lon_index + 1) as f64 * res)
        }
    }
}