use std::{
    fs::File,
    io::{BufRead, BufReader},
    sync::Mutex,
};

use anyhow::Context;
use chrono::{NaiveDate, Utc};

use crate::{
    geo::GeoPoint,
    math::{factorial_ratio, legendre::LegendreManager},
    units::{Angle, Unit},
};

use super::{
    MagneticField, MagneticFieldElements, MagneticModelCoefficients, EARTH_WGS84_RECIPROCAL_FLATTENING, EARTH_WGS84_SEMI_MAJOR_AXIS,
    GEOMAGNETIC_REFERENCE_RADIUS, WMM_EXPANSION,
};

/// Represents a Magnetic Model
pub struct MagneticModel {
    /// Epoch: A decimal year
    pub epoch: f64,
    pub name: String,
    pub release_date: NaiveDate,
    coeffs: Mutex<Vec<Vec<MagneticModelCoefficients>>>,
    legendre_manager: LegendreManager,
}

impl MagneticModel {
    pub fn new(epoch: f64, name: &String, release_date: &NaiveDate, coeffs: &Vec<MagneticModelCoefficients>) -> MagneticModel {
        let mut new_coeffs: Vec<Vec<MagneticModelCoefficients>> = Vec::new();

        // Initialize vector
        for n in 0..WMM_EXPANSION + 1 {
            let mut coeff_row: Vec<MagneticModelCoefficients> = Vec::new();

            for m in 0..WMM_EXPANSION + 1 {
                coeff_row.push(MagneticModelCoefficients {
                    n,
                    m,
                    g_nm: 0.0,
                    h_nm: 0.0,
                    g_dot_nm: 0.0,
                    h_dot_nm: 0.0,
                })
            }
            new_coeffs.push(coeff_row);
        }

        // Add coeffs passed in
        for (_i, coeff) in coeffs.iter().enumerate() {
            if coeff.n <= WMM_EXPANSION && coeff.m <= WMM_EXPANSION {
                new_coeffs[coeff.n][coeff.m] = coeff.clone();
            }
        }

        return MagneticModel {
            epoch,
            name: name.clone(),
            release_date: release_date.clone(),
            coeffs: Mutex::new(new_coeffs),
            legendre_manager: LegendreManager::new(),
        };
    }

    /// Loads the Magnetic Model from a *.COF file
    pub fn from_file(filename: &str) -> anyhow::Result<MagneticModel> {
        // Open file
        let file = File::open(filename).with_context(|| format!("Failed to read magnetic file {}", filename))?;

        // Created buffered reader
        let reader = BufReader::new(file);

        // Initial Values
        let mut epoch_date = 0.0;
        let mut model_name = "".to_string();
        let mut release_date = Utc::now().date_naive();
        let mut coeffs: Vec<MagneticModelCoefficients> = Vec::new();

        // Loop through file
        let mut i = 0;
        for line in reader.lines() {
            if let Ok(line_str) = line {
                // Split line by whitespace
                let mut split = line_str.split_whitespace();

                // Check for header line
                if (i == 0) {
                    // Parse header line
                    let error_val = "Invalid COF Header Format";
                    epoch_date = split.next().context(error_val)?.parse::<f64>()?;
                    model_name = split.next().context(error_val)?.to_string();
                    let string_date = split.next().context(error_val)?;

                    release_date = NaiveDate::parse_from_str(string_date, "%m/%d/%Y")?;
                } else {
                    let error_context = "Invalid COF Coefficient Line Format";
                    let n = match split.next().context(error_context)?.parse::<usize>() {
                        Ok(val) => val,
                        Err(_) => break
                    };                    
                    let m = split.next().context(error_context)?.parse::<usize>()?;
                    let g_nm = split.next().context(error_context)?.parse::<f64>()?;
                    let h_nm = split.next().context(error_context)?.parse::<f64>()?;
                    let g_dot_nm = split.next().context(error_context)?.parse::<f64>()?;
                    let h_dot_nm = split.next().context(error_context)?.parse::<f64>()?;

                    if (n > 9999) {
                        // Assume end of file
                        break;
                    }

                    coeffs.push(MagneticModelCoefficients {
                        n,
                        m,
                        g_nm,
                        h_nm,
                        g_dot_nm,
                        h_dot_nm,
                    });
                }
            }
            i += 1;
        }

        return Ok(MagneticModel::new(epoch_date, &model_name, &release_date, &coeffs));
    }

    /// Returns the magnetic coefficients for n and m
    pub fn coeffs(&self, n: usize, m: usize) -> MagneticModelCoefficients {
        if (n > WMM_EXPANSION || m > WMM_EXPANSION) {
            return MagneticModelCoefficients {
                n,
                m,
                g_nm: 0.0,
                h_nm: 0.0,
                g_dot_nm: 0.0,
                h_dot_nm: 0.0,
            };
        }

        let mutex_guard = self.coeffs.lock().unwrap();
        return mutex_guard[n][m];
    }

    fn convert_to_spherical(point: &GeoPoint) -> (f64, f64, f64, f64) {
        // Equations 7-8
        let phi = point.lat.as_radians();
        let lambda = point.lon.as_radians();
        let h = point.alt.as_meters();

        let sin_phi = f64::sin(phi);
        let cos_phi = f64::cos(phi);

        // Calculate Rc
        let f = 1.0 / EARTH_WGS84_RECIPROCAL_FLATTENING;
        let e_2 = f * (2.0 - f);
        let r_c = EARTH_WGS84_SEMI_MAJOR_AXIS / f64::sqrt(1.0 - e_2 * sin_phi.powi(2));

        let p = (r_c + h) * cos_phi;
        let z = (r_c * (1.0 - e_2) + h) * sin_phi;
        let r = f64::sqrt(p.powi(2) + z.powi(2));
        let phi_prime = f64::asin(z / r);

        return (lambda, phi, phi_prime, r);
    }

    /// Calculates the Magnetic Field at a point and time
    pub fn calculate_field(&self, point: &GeoPoint, date: &NaiveDate) -> MagneticField {
        let t = date;
        let t_0 = self.epoch;

        // Create return values
        let mut field_elems = MagneticFieldElements::default();
        let mut sec_elems = MagneticFieldElements::default();

        // Convert to spherical
        let (lambda, phi, phi_prime, r) = Self::convert_to_spherical(point);

        // Calculate Field Values
        // Equations 9-16
        let sin_phi = f64::sin(phi_prime);
        let cos_phi = f64::cos(phi_prime);
        let mut g;
        let mut h;
        let mut dg;
        let mut dh;

        // Step 3 in WMM Technical Report
        for n in 1..WMM_EXPANSION as i32 {
            let nn = n + 1;
            let f = (GEOMAGNETIC_REFERENCE_RADIUS / r).powi(n + 2);

            for m in 0..n + 1 {
                // Schmidt Semi-Normalized Value
                let mut p = self.legendre_manager.legendre_function(n, m, sin_phi);
                let mut q = self.legendre_manager.legendre_function(n + 1, m, sin_phi);

                if (m > 0) {
                    p *= f64::sqrt(2.0 / factorial_ratio(n + m, n - m));
                    q *= f64::sqrt(2.0 / factorial_ratio(n + m, n - m));
                }

                // Equation 16
                let dp = nn as f64 * f64::tan(phi_prime) * p - (nn - m) as f64 / cos_phi * q;

                // Equation 9
                let wmm_coefficients = self.coeffs(n as usize, m as usize).get_point_on_date(t_0, t);

                g = wmm_coefficients.g_nm;
                h = wmm_coefficients.h_nm;
                dg = wmm_coefficients.g_dot_nm;
                dh = wmm_coefficients.h_dot_nm;

                // Calculate field elements
                let sinMLambda = f64::sin(m as f64 * lambda);
                let cosMLambda = f64::cos(m as f64 * lambda);
                // Equation 10
                field_elems.x += -f * (g * cosMLambda + h * sinMLambda) * dp;
                // Equation 11
                field_elems.y += f / cos_phi * m as f64 * (g * sinMLambda - h * cosMLambda) * p;
                // Equation 12
                field_elems.z += -nn as f64 * f * (g * cosMLambda + h * sinMLambda) * p;
                // Equation 13
                sec_elems.x += -f * (dg * cosMLambda + dh * sinMLambda) * dp;
                // Equation 14
                sec_elems.y += f / cos_phi * m as f64 * (dg * sinMLambda - dh * cosMLambda) * p;
                // Equation 15
                sec_elems.z += -nn as f64 * f * (dg * cosMLambda + dh * sinMLambda) * p;
            }
        }

        // Rotate Magnetic Field
        // Equation 17
        field_elems.x = field_elems.x * f64::cos(phi_prime - phi) - field_elems.z * f64::sin(phi_prime - phi);
        field_elems.z = field_elems.x * f64::sin(phi_prime - phi) + field_elems.z * f64::cos(phi_prime - phi);

        // Equation 18
        sec_elems.x = sec_elems.x * f64::cos(phi_prime - phi) - sec_elems.z * f64::sin(phi_prime - phi);
        sec_elems.z = sec_elems.x * f64::sin(phi_prime - phi) + sec_elems.z * f64::cos(phi_prime - phi);

        // Calculate derived values
        // Equation 19
        field_elems.h = f64::sqrt(field_elems.x.powi(2) + field_elems.y.powi(2));
        field_elems.f = f64::sqrt(field_elems.h.powi(2) + field_elems.z.powi(2));
        field_elems.incl = Angle::new(f64::atan2(field_elems.z, field_elems.h));
        field_elems.decl = Angle::new(f64::atan2(field_elems.y, field_elems.x));

        // Equation 20
        sec_elems.h = (field_elems.x * sec_elems.x + field_elems.y * sec_elems.y) / field_elems.h;
        sec_elems.f = (field_elems.x * sec_elems.x + field_elems.y * sec_elems.y + field_elems.z * sec_elems.z) / field_elems.f;
        sec_elems.incl = Angle::new((field_elems.h * sec_elems.z - field_elems.z * sec_elems.h) / field_elems.f.powi(2));
        sec_elems.decl = Angle::new((field_elems.x * sec_elems.y - field_elems.y * sec_elems.x) / field_elems.h.powi(2));

        // Return result
        return MagneticField {
            field_elements: field_elems,
            sec_elements: sec_elems,
        };
    }
}
