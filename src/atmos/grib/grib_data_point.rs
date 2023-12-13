use std::fmt::Display;

use crate::{geo::{Latitude, Longitude, Bearing, GeoPoint, EARTH_RADIUS}, units::{Pressure, Length, Temperature, Velocity, Unit}, atmos::ISA_STD_PRES};

/// Data Point containing Grib information
#[derive(Clone, Copy, Default, PartialEq)]
pub struct GribDataPoint {
    pub lat: Latitude,
    pub lon: Longitude,
    pub geo_pot_height: Length,
    pub level_pressure: Pressure,
    pub temp: Temperature,
    pub v: Velocity,
    pub u: Velocity,
    pub rh: f64,
    pub sfc_press: Pressure,
}

impl Display for GribDataPoint {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let wind = self.wind();
        return write!(
            f,
            "Lat: {:.3} Lon: {:.3} Level: {:.0}hPa Height: {:.0}ft Temp: {:.1}C Wind: {:03.0}@{:.0}KT RH: {}",
            self.lat.as_degrees(),
            self.lon.as_degrees(),
            self.level_pressure.as_hectopascals(),
            self.geo_pot_height.as_feet(),
            self.temp.as_celsius(),
            wind.0.as_degrees(),
            wind.1.as_knots(),
            self.rh
        );
    }
}

impl std::fmt::Debug for GribDataPoint {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        return std::fmt::Display::fmt(&self, f);
    }
}

impl GribDataPoint {
    pub fn new(lat: Latitude, lon: Longitude, level_pressure: Pressure) -> GribDataPoint {
        return GribDataPoint {
            lat,
            lon,
            geo_pot_height: Length::new(0.0),
            level_pressure,
            temp: Temperature::from_celsius(15.0),
            v: Velocity::new(0.0),
            u: Velocity::new(0.0),
            rh: 0.0,
            sfc_press: ISA_STD_PRES,
        };
    }

    pub fn wind(&self) -> (Bearing, Velocity) {
        let wind_dir = Bearing::from_radians(f64::atan2(-self.u.as_meters_per_second(), -self.v.as_meters_per_second()));
        let wind_spd = Velocity::sqrt(self.u.powi(2) + self.v.powi(2));

        return (wind_dir, wind_spd);
    }

    pub fn distance_from(&self, point: &GeoPoint) -> Length {
        let acft_geo_pot_height = EARTH_RADIUS * point.alt / (EARTH_RADIUS + point.alt);
        let flat_dist = GeoPoint::flat_distance(point, &GeoPoint::new(self.lat, self.lon, point.alt));
        let alt_dist = (acft_geo_pot_height - self.geo_pot_height).abs();

        return Length::sqrt(flat_dist.powi(2) + alt_dist.powi(2));
    }
}