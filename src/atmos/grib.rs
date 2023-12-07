use std::{
    env::temp_dir,
    fmt::Display,
    fs::{remove_file, create_dir_all},
    path::{Path, PathBuf},
    sync::Mutex, thread::{self, JoinHandle},
};

use chrono::{DateTime, Duration, Timelike, Utc};

use crate::{
    geo::{
        bearing::Bearing,
        geo_point::GeoPoint,
        geo_tile::{GeoTile, GeoTileBounds},
        latitude::Latitude,
        longitude::Longitude,
        EARTH_RADIUS,
    },
    units::{angle::Angle, length::Length, pressure::Pressure, temperature::Temperature, velocity::Velocity},
};

use super::ISA_STD_PRES;

pub struct GribTile {
    bounds: GeoTileBounds,
    data_points: Mutex<Vec<GribDataPoint>>,
    downloaded: bool,
    forecast_date_utc: DateTime<Utc>,
    download_path: PathBuf,
    download_thread_handle: Option<JoinHandle<()>>
}

impl GeoTile for GribTile {
    fn bottom_lat(&self) -> Latitude {
        return self.bounds.bottom_lat();
    }

    fn top_lat(&self) -> Latitude {
        return self.bounds.top_lat();
    }

    fn left_lon(&self) -> Longitude {
        return self.bounds.left_lon();
    }

    fn right_lon(&self) -> Longitude {
        return self.bounds.right_lon();
    }
}

impl Drop for GribTile {
    fn drop(&mut self) {
        if (self.downloaded) {
            // Remove file
            remove_file(self.grib_file_name()).ok();
        }
    }
}

impl GribTile {
    pub fn new(point: &GeoPoint, date_time: &DateTime<Utc>, download_path: &PathBuf) -> GribTile {
        // Set default download path
        //let path = temp_dir().join("aviationcalc").join("gribtiles");

        // Create tile
        let mut new_tile = GribTile {
            downloaded: false,
            bounds: GeoTileBounds::new_from_point(point, Angle::from_degrees(1.0)),
            data_points: Mutex::new(Vec::new()),
            forecast_date_utc: date_time.clone(),
            download_path: download_path.clone(),
            download_thread_handle: None
        };

        // Start download asynchronously
        let dl_handle = thread::spawn(move || {
            new_tile.download();
        });
        //new_tile.download_thread_handle = Some(dl_handle);

        return new_tile;
    }

    fn offset_date_utc(&self) -> DateTime<Utc> {
        return self.forecast_date_utc - Duration::hours(6);
    }

    fn cycle(&self) -> i16 {
        let td_hours = self.offset_date_utc().hour();

        return (td_hours / 6 * 6) as i16;
    }

    fn forecast_hour(&self) -> i16 {
        return (self.offset_date_utc().hour() as i16 - self.cycle() + 6 as i16);
    }

    pub fn forecast_date_utc(&self) -> DateTime<Utc> {
        return self.forecast_date_utc;
    }

    pub fn grib_file_name(&self) -> PathBuf {
        let filename = format!(
            "GribTile_{}_t{:02}z_f{:03}_l{}_t{}_r{}_b{}.grb",
            self.offset_date_utc().format("%Y%m%d"),
            self.cycle(),
            self.forecast_hour(),
            self.left_lon().as_degrees() as i16,
            self.top_lat().as_degrees() as i16,
            self.right_lon().as_degrees() as i16,
            self.bottom_lat().as_degrees() as i16
        );

        return self.download_path.join(filename);
    }

    pub fn download_url(&self) -> String {
        // Generate URL
        return format!(
            "https://nomads.ncep.noaa.gov/cgi-bin/filter_gfs_0p25.pl\
            ?file=gfs.t{:02}z.pgrb2.0p25.f{:03}\
            &lev_1000_mb=on&lev_100_mb=on&lev_150_mb=on&lev_200_mb=on&lev_250_mb=on&lev_300_mb=on\
            &lev_350_mb=on&lev_400_mb=on&lev_450_mb=on&lev_500_mb=on&lev_550_mb=on&lev_600_mb=on\
            &lev_650_mb=on&lev_700_mb=on&lev_750_mb=on&lev_800_mb=on&lev_850_mb=on&lev_900_mb=on\
            &lev_925_mb=on&lev_950_mb=on&lev_975_mb=on&lev_mean_sea_level=on&lev_surface=on&var_HGT=on\
            &var_PRES=on&var_TMP=on&var_UGRD=on&var_VGRD=on&var_PRMSL=on&subregion=\
            &leftlon={}&rightlon={}&toplat={}&bottomlat={}\
            &dir=%2Fgfs.{}%2F{:02}%2Fatmos",
            self.cycle(),
            self.forecast_hour(),
            self.left_lon().as_degrees() as i16,
            self.right_lon().as_degrees() as i16,
            self.top_lat().as_degrees() as i16,
            self.bottom_lat().as_degrees() as i16,
            self.offset_date_utc().format("%Y%m%d"),
            self.cycle()
        );
    }

    pub fn closest_point(&self, point: &GeoPoint) -> Option<GribDataPoint> {
        if (!self.downloaded) {
            return None;
        }

        let mut min_dist = Length::new(f64::MAX);
        let mut ret_val = None;

        let mutex_guard = self.data_points.lock().unwrap();

        for (_i, grib_point) in mutex_guard.iter().enumerate() {
            let dist = grib_point.distance_from(point);
            if (ret_val.is_none() || dist < min_dist) {
                ret_val = Some(grib_point.clone());
                min_dist = dist;
            }
        }

        return ret_val;
    }

    pub fn is_valid(&self, date_time: &DateTime<Utc>) -> bool {
        let td = self.forecast_date_utc - date_time;
        let td_c = Duration::hours(1);
        return td.abs().num_hours() < td_c.num_hours();
    }

    fn download(&mut self) {
        if (!self.downloaded) {
            let filename = self.grib_file_name();

            // Make sure file doesnt exist or remove it
            remove_file(filename).ok();

            // Generate url
            let url = self.download_url();

            // Create folder if it doesn't exist
            create_dir_all(self.download_path);
        }
    }
}

/// Data Point containing Grib information
#[derive(Clone, Copy, Default, PartialEq, Debug)]
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
            "Lat: {} Lon: {} Level: {}hPa Height: {}ft Temp: {}C Wind: {}@{}KT RH: {}",
            self.lat,
            self.lon,
            self.level_pressure.as_hectopascals(),
            self.geo_pot_height.as_feet(),
            self.temp.as_celsius(),
            wind.0.as_degrees(),
            wind.1.as_knots(),
            self.rh
        );
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
