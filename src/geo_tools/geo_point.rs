use std::{f64::consts::PI, ops::Sub};

use crate::units::{angle::Angle, length::Length};

use super::{bearing::Bearing, geo_util, latitude::Latitude, longitude::Longitude};

/// Represents a 3D Coordinate Point on the globe.
#[derive(Clone, Copy, Default, PartialEq, Debug)]
pub struct GeoPoint {
    pub lat: Latitude,
    pub lon: Longitude,
    pub alt: Length,
}

impl Sub for GeoPoint {
    type Output = Length;

    fn sub(self, rhs: Self) -> Self::Output {
        return self.distance(&rhs);
    }
}

impl GeoPoint {
    /// Creates a new GeoPoint
    pub fn new(lat: Latitude, lon: Longitude, alt: Length) -> GeoPoint {
        GeoPoint { lat, lon, alt }
    }

    /// Moves the GeoPoint on a bearing by a distance
    pub fn move_by(&mut self, bearing: Bearing, distance: Length) {
        let r = geo_util::EARTH_RADIUS + self.alt;
        let bearing_rads = bearing.as_radians();
        let lat1 = self.lat.as_radians();
        let lon1 = self.lon.as_radians();

        let lat2 = f64::asin(
            f64::sin(lat1) * f64::cos((distance / r).as_meters()) + f64::cos(lat1) * f64::sin((distance / r).as_meters()) * f64::cos(bearing_rads),
        );

        let lon2 = lon1
            + f64::atan2(
                f64::sin(bearing_rads) * f64::sin((distance / r).as_meters()) * f64::cos(lat1),
                f64::cos((distance / r).as_meters()) - f64::sin(lat1) * f64::sin(lat2),
            );

        self.lat = Latitude(Angle(lat2));
        self.lon = Longitude(Angle(lon2));
    }

    /// Calculates the flat distance between two GeoPoints
    pub fn flat_distance(&self, other: &GeoPoint) -> Length {
        let phi1 = self.lat.as_radians();
        let phi2 = other.lat.as_radians();
        let delta_phi = other.lat.as_radians() - self.lat.as_radians();
        let delta_lambda = other.lon.as_radians() - self.lon.as_radians();

        let a = f64::sin(delta_phi / 2.0).powi(2) + f64::cos(phi1) * f64::cos(phi2) * f64::sin(delta_lambda / 2.0).powi(2);

        // Check for opposite end of planet
        if (1.0 - a <= 0.0) {
            // Return half circumference
            return geo_util::EARTH_RADIUS * PI;
        }

        let c = 2.0 * f64::atan2(f64::sqrt(a), f64::sqrt(1.0 - a));

        let d = geo_util::EARTH_RADIUS * c;

        return d;
    }

    /// Calculates the distance including altitude between two GeoPoints
    pub fn distance(&self, other: &GeoPoint) -> Length {
        let flat_dist = self.flat_distance(other);
        let alt_dist = (other.alt - self.alt).abs();

        return Length::sqrt(flat_dist.powi(2) + alt_dist.powi(2));
    }

    /// Calculates the intersection between two points and two courses
    /// 
    /// *Note:* This function will not account for reciprocal of the bearings
    pub fn intersection(&self, bearing_1: Bearing, other: &GeoPoint, bearing_2: Bearing) -> Option<GeoPoint> {
        // Conversions to radians
        let phi_1 = self.lat.as_radians();
        let phi_2 = other.lat.as_radians();
        let lambda_1 = self.lon.as_radians();
        let lambda_2 = other.lon.as_radians();
        let theta_1_3 = bearing_1.as_radians();
        let theta_2_3 = bearing_2.as_radians();
        let delta_phi = phi_2 - phi_1;
        let delta_lambda = lambda_2 - lambda_1;

        // Angular distance (lat1, lon1) - (lat2, lon2)
        let sigma_1_2 = 2.0
            * f64::asin(f64::sqrt(
                f64::sin(delta_phi / 2.0).powi(2) + f64::cos(phi_1) * f64::cos(phi_2) * f64::sin(delta_lambda / 2.0).powi(2),
            ));

        // Coincident Points
        if (sigma_1_2 < f64::EPSILON) {
            return Some(self.clone());
        }

        // Initial/Final bearing between points
        let cos_theta_a = (f64::sin(phi_2) - f64::sin(phi_1) * f64::cos(sigma_1_2)) / (f64::sin(sigma_1_2) * f64::cos(phi_1));
        let cos_theta_b = (f64::sin(phi_1) - f64::sin(phi_2) * f64::cos(sigma_1_2)) / (f64::sin(sigma_1_2) * f64::cos(phi_2));
        let theta_a = f64::acos(f64::min(f64::max(cos_theta_a, -1.0), 1.0)); // Prevent rounding errors
        let theta_b = f64::acos(f64::min(f64::max(cos_theta_b, -1.0), 1.0)); // Prevent rounding errors

        let theta_1_2 = if f64::sin(delta_lambda) > 0.0 { theta_a } else { 2.0 * PI - theta_a };
        let theta_2_1 = if f64::sin(delta_lambda) > 0.0 { 2.0 * PI - theta_b } else { theta_b };

        let alpha_1 = theta_1_3 - theta_1_2; // Angle 2-1-3
        let alpha_2 = theta_2_1 - theta_2_3; // Angle 1-2-3

        // Infinite intersections
        if (f64::sin(alpha_1) == 0.0 && f64::sin(alpha_2) == 0.0) {
            return None;
        }

        // Ambiguous Intersection (antipodal?)
        if (f64::sin(alpha_1) * f64::sin(alpha_2) < 0.0) {
            return None;
        }

        let cos_alpha_3 = -f64::cos(alpha_1) * f64::cos(alpha_2) + f64::sin(alpha_1) * f64::sin(alpha_2) * f64::cos(sigma_1_2);

        let sigma_1_3 = f64::atan2(
            f64::sin(sigma_1_2) * f64::sin(alpha_1) * f64::sin(alpha_2),
            f64::cos(alpha_2) + f64::cos(alpha_1) * cos_alpha_3,
        );

        let phi_3 = f64::asin(f64::min(
            f64::max(
                f64::sin(phi_1) * f64::cos(sigma_1_3) + f64::cos(phi_1) * f64::sin(sigma_1_3) * f64::cos(theta_1_3),
                -1.0,
            ),
            1.0,
        ));

        let delta_lambda_1_3 = f64::atan2(
            f64::sin(theta_1_3) * f64::sin(sigma_1_3) * f64::cos(phi_1),
            f64::cos(sigma_1_3) - f64::sin(phi_1) * f64::sin(phi_3),
        );

        let lambda_3 = lambda_1 + delta_lambda_1_3;

        return Some(GeoPoint::new(Latitude(Angle(phi_3)), Longitude(Angle(lambda_3)), Length(0.0)));
    }

    /// Find closest intersection between two points and two radials.
    /// 
    /// This function will try both the radial and the reciprocal
    pub fn closest_intersection(&self, radial_1: Bearing, other: &GeoPoint, radial_2: Bearing) -> Option<GeoPoint> {
        // Try all possible radials
        let mut intersections = Vec::new();
        intersections.push(GeoPoint::intersection(&self, radial_1, other, radial_2));
        intersections.push(GeoPoint::intersection(&self, radial_1, other, -radial_2));
        intersections.push(GeoPoint::intersection(&self, -radial_1, other, radial_2));
        intersections.push(GeoPoint::intersection(&self, -radial_1, other, -radial_2));

        let mut closest_dist = Length(f64::MAX);
        let mut closest_intersection = None;

        for (_i, arg) in intersections.iter().enumerate() {
            if let Some(this_intersection) = arg {
                let dist = self.flat_distance(&this_intersection);

                if dist < closest_dist {
                    closest_intersection = Some(this_intersection.to_owned());
                    closest_dist = dist;
                }
            }
        }

        return closest_intersection;
    }

    /// Calculates initial bearing from one GeoPoint to other
    pub fn initial_bearing(&self, other: &GeoPoint) -> Bearing {
        // Convert to Radians
        let phi_1 = self.lat.as_radians();
        let phi_2 = other.lat.as_radians();
        let lambda_1 = self.lon.as_radians();
        let lambda_2 = self.lon.as_radians();

        // Find angle between the two
        let y = f64::sin(lambda_2 - lambda_1) * f64::cos(phi_2);
        let x = f64::cos(phi_1) * f64::sin(phi_2) - f64::sin(phi_1) * f64::cos(phi_2) * f64::cos(lambda_2 - lambda_1);

        let theta = f64::atan2(y, x);

        // Convert from -180, +180 to 0, 359
        return Bearing(Angle(theta));
    }

    /// Calculates final bearing from one GeoPoint to other
    pub fn final_bearing(&self, other: &GeoPoint) -> Bearing {
        // Calculate initial bearing from end to start and reverse
        return -other.initial_bearing(self);
    }
}
