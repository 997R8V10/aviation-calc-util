#![allow(unused_parens)]
#![allow(non_snake_case)]

pub use chrono::NaiveDate;
pub use chrono::DateTime;
pub use chrono::Utc;

mod macros;
pub mod math;
pub mod geo;
pub mod aviation;
pub mod atmos;
pub mod magnetic;
pub mod units;

#[cfg(test)]
mod tests;