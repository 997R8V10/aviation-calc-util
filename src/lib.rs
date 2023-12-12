#![allow(unused_parens)]
#![allow(non_snake_case)]

#![doc = include_str!(concat!(env!("CARGO_MANIFEST_DIR"), "/README.md"))]

pub use chrono::NaiveDate;
pub use chrono::DateTime;
pub use chrono::Utc;
pub use chrono::Datelike;
pub use anyhow::Result;
pub use anyhow::Error;

mod macros;

/// Math Tools and Calculations
pub mod math;

/// Geographic Calculations
pub mod geo;

/// Aviation Related Calculations
pub mod aviation;

/// Atmospheric Calculations
pub mod atmos;

/// Magnetic Calculations
pub mod magnetic;

/// Unit Conversions and Handling
pub mod units;

#[cfg(test)]
mod tests;