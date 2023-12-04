use chrono::{NaiveDate, Datelike};

pub mod magnetic_model;

pub fn get_epoch_year(date: &NaiveDate) -> f64 {
    let year = date.year();
    let day = date.ordinal0();
    let total_days_in_year = if date.leap_year() {365.0} else {366.0};
    let days_percentage = day as f64 / total_days_in_year as f64;

    return year as f64 + days_percentage;
}