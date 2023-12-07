use std::{
    fmt::Display,
    fs::{create_dir_all, remove_file, File},
    io::{self, BufReader},
    path::PathBuf,
    sync::Mutex,
    thread::{self, JoinHandle},
};

use chrono::{DateTime, Duration, Timelike, Utc};
use grib::Grib2SubmessageDecoder;

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
    forecast_date_utc: DateTime<Utc>,
    download_path: PathBuf,
    download_thread_handle: Option<JoinHandle<anyhow::Result<Vec<GribDataPoint>>>>,
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
        if let Some(dl_handle) = self.download_thread_handle.take() {
            // Join the thread
            dl_handle.join().ok();
        }

        // Remove file
        remove_file(self.grib_file_name()).ok();
    }
}

impl GribTile {
    pub fn new(point: &GeoPoint, date_time: &DateTime<Utc>, download_path: &PathBuf) -> GribTile {
        // Set default download path
        //let path = temp_dir().join("aviationcalc").join("gribtiles");

        // Create tile
        let mut new_tile = GribTile {
            bounds: GeoTileBounds::new_from_point(point, Angle::from_degrees(1.0)),
            data_points: Mutex::new(Vec::new()),
            forecast_date_utc: date_time.clone(),
            download_path: download_path.clone(),
            download_thread_handle: None,
        };

        let filename = new_tile.grib_file_name();
        let download_url = new_tile.download_url();
        let download_path_new = download_path.clone();

        // Start download asynchronously
        let dl_handle = thread::spawn(move || GribTile::download_and_extract_data(&download_path_new, &filename, &download_url));
        new_tile.download_thread_handle = Some(dl_handle);

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

    pub fn closest_point(&mut self, point: &GeoPoint) -> Option<GribDataPoint> {
        // Check if download has finished
        if let Some(dl_handle) = self.download_thread_handle.as_mut() {
            if (!dl_handle.is_finished()) {
                return None;
            }
        }

        // Take download handle
        if let Some(thread_handle) = self.download_thread_handle.take() {
            if let Ok(data_points) = thread_handle.join().unwrap() {
                let mut lock_guard = self.data_points.lock().unwrap();

                *lock_guard = data_points;
            }
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

    /// Downloads a grib tile, then extracts the GribDataPoints
    ///
    /// **Parameters:**
    /// - `download_path`: Download directory
    /// - `filename`: Target full file name
    /// - `url`: Grib File URL
    fn download_and_extract_data(download_path: &PathBuf, filename: &PathBuf, url: &String) -> anyhow::Result<Vec<GribDataPoint>> {
        // Make sure file doesnt exist or remove it
        remove_file(filename).ok();

        // Create folder if it doesn't exist
        create_dir_all(download_path)?;

        // Download GRIB file
        let mut resp = ureq::get(url).call()?.into_reader();
        let mut out = File::create(filename)?;
        io::copy(&mut resp, &mut out)?;

        // Extract Data
        // Open File
        let file_reader = BufReader::new(File::open(filename)?);

        // Get grib
        let grib2 = grib::from_reader(file_reader)?;

        // Create return list
        let mut ret_list: Vec<GribDataPoint> = Vec::new();
        let mut sfc_values: Vec<GribDataPoint> = Vec::new();

        // GRIB specific values
        let missing_value = 1e+20;

        // Loop through all messages in a file
        for (_index, submessage) in grib2.iter() {
            // Check level type
            if let Some(lvl_types) = submessage.prod_def().fixed_surfaces() {
                // Get level
                let level_type = lvl_types.0.surface_type;
                let level = Pressure::new(lvl_types.0.scaled_value as f64);
                let mut is_sfc_msg = false;
                let mut is_isobaric_msg = false;

                if (level_type == 100) {
                    // Isobaric
                    is_isobaric_msg = true;
                } else if (level_type == 101 || level_type == 1) {
                    // MSL or PRMSL
                    is_sfc_msg = true;
                }

                if (is_isobaric_msg || is_sfc_msg) {
                    // Get parameter
                    if let Some(param_category) = submessage.prod_def().parameter_category() {
                        if let Some(param_num) = submessage.prod_def().parameter_number() {
                            // Loop through values
                            let latlons = submessage.latlons()?;
                            let decoder = Grib2SubmessageDecoder::from(submessage)?;
                            let values = decoder.dispatch()?;

                            for ((lat, lon), value) in latlons.zip(values) {
                                // Ignore missing values
                                if (value == missing_value as f32) {
                                    continue;
                                }

                                let latitude = Latitude::from_degrees(lat as f64);
                                let longitude = Longitude::from_degrees(lon as f64);

                                // Get grid point if it exists
                                let found_point = {
                                    let search_list = if is_isobaric_msg { &mut ret_list } else { &mut sfc_values };
                                    let mut list_iter = search_list.iter_mut();

                                    loop {
                                        if let Some(point) = list_iter.next() {
                                            if point.lat == latitude && point.lon == longitude && point.level_pressure == level {
                                                break point;
                                            }
                                        } else {
                                            search_list.push(GribDataPoint::new(latitude, longitude, level));

                                            break search_list.last_mut().unwrap();
                                        }
                                    }
                                };

                                // Set Values
                                if (is_isobaric_msg) {
                                    match (param_category, param_num) {
                                        // u (wind)
                                        (2, 2) => found_point.u = Velocity::new(value as f64),
                                        // v (wind)
                                        (2, 3) => found_point.v = Velocity::new(value as f64),
                                        // t (temperature)
                                        (0, 0) => found_point.temp = Temperature::new(value as f64),
                                        // gh (geopotential height)
                                        (3, 5) => found_point.geo_pot_height = Length::new(value as f64),
                                        _ => {}
                                    }
                                } else {
                                    match (param_category, param_num) {
                                        // pressure
                                        (3, 0) => found_point.sfc_press = Pressure::new(value as f64),
                                        _ => {}
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        // Add Surface pressures back in
        for point in ret_list.iter_mut() {
            for sfc_point in sfc_values.iter() {
                if (sfc_point.lat == point.lat && sfc_point.lon == point.lon) {
                    point.sfc_press = sfc_point.sfc_press;
                }
            }
        }

        return Ok(ret_list);
    }
}

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
