use chrono::{NaiveDate, Datelike};

mod magnetic_model;
mod magnetic_field;
mod magnetic_model_coefficients;
mod magnetic_tile;

pub use magnetic_model::MagneticModel;
pub use magnetic_field::MagneticField;
pub use magnetic_field::MagneticFieldElements;
pub use magnetic_model_coefficients::MagneticModelCoefficients;
pub use magnetic_tile::MagneticTile;
pub use magnetic_tile::MagneticTileManager;

pub const EARTH_WGS84_SEMI_MAJOR_AXIS: f64 = 6378137.0;
pub const EARTH_WGS84_RECIPROCAL_FLATTENING: f64 = 298.257223563;
pub const WMM_EXPANSION: usize = 12;
pub const GEOMAGNETIC_REFERENCE_RADIUS: f64 = 6371200.0;

pub fn get_epoch_year(date: &NaiveDate) -> f64 {
    let year = date.year();
    let day = date.ordinal0();
    let total_days_in_year = if date.leap_year() {365.0} else {366.0};
    let days_percentage = day as f64 / total_days_in_year as f64;

    return year as f64 + days_percentage;
}