use crate::units::{Angle, Length};

mod bearing;
pub use bearing::Bearing;
mod geo_point;
pub use geo_point::GeoPoint;
mod latitude;
pub use latitude::Latitude;
mod geo_tile;
mod longitude;
pub use geo_tile::GeoTile;
pub use geo_tile::GeoTileBounds;
pub use longitude::Longitude;
//mod altitude;
//pub use altitude::Altitude;

// Constants
/// Average radius of the Earth
pub const EARTH_RADIUS: Length = Length(6371e3);

/// Earth Surface Gravitational Acceleration (m/s^2)
pub const EARTH_GRAVITY: f64 = 9.80665;

// Coordinate conversions
fn convert_nats_to_decimal_single(nats_coord: &str, is_latitude: bool) -> Option<Angle> {
    let length = if is_latitude { 7 } else { 8 };
    let last_char = &nats_coord[length - 1..];
    let sign = if last_char == "S" || last_char == "W" { -1 } else { 1 };
    let mut current_pos = 0;
    let first_chunk = if is_latitude { 2 } else { 3 };
    if let Ok(mut degrees) = &nats_coord[current_pos..current_pos + first_chunk].parse::<i32>() {
        degrees *= sign;
        current_pos += first_chunk;
        if let Ok(minutes) = &nats_coord[current_pos..current_pos + 2].parse::<u32>() {
            current_pos += 2;
            if let Ok(seconds) = &nats_coord[current_pos..current_pos + 2].parse::<f64>() {
                return Some(Angle::from_deg_min_sec(degrees, *minutes, *seconds));
            }
        }
    }

    return None;
}

fn convert_decimal_to_nats_single(decimal_coord: Angle, is_latitude: bool) -> String {
    let (mut degrees, mut minutes, mut seconds) = decimal_coord.as_deg_min_sec();

    // Determine which letter to append at the end of the return value
    let mut dir_letter = if degrees < 0 {
        if is_latitude {
            'S'
        } else {
            'W'
        }
    } else {
        if is_latitude {
            'N'
        } else {
            'E'
        }
    };

    degrees = degrees.abs();

    // Round seconds
    seconds = seconds.round();

    if seconds >= 60.0 {
        seconds = 0.0;
        minutes += 1;
        if minutes >= 60 {
            degrees += 1;
            if is_latitude && degrees > 90 {
                degrees = 90 - (degrees - 90);
                if (dir_letter == 'N') {
                    dir_letter = 'S';
                } else {
                    dir_letter = 'N';
                }
            } else if !is_latitude && degrees > 180 {
                degrees = 180 - (degrees - 180);
                if (dir_letter == 'E') {
                    dir_letter = 'W';
                } else {
                    dir_letter = 'E';
                }
            }
        }
    }

    let seconds_i = seconds as i32;

    // Format the return value of the string with the appropriate level of zero padding
    let value = if (is_latitude) {
        format!("{:02}{:02}{:02}{}", degrees, minutes, seconds_i, dir_letter).to_string()
    } else {
        format!("{:03}{:02}{:02}{}", degrees, minutes, seconds_i, dir_letter).to_string()
    };

    return value;
}

fn convert_decimal_deg_to_vrc_single(decimal_coord: Angle, is_latitude: bool) -> String {
    let (mut degrees, minutes, seconds) = decimal_coord.as_deg_min_sec();
    degrees = degrees.abs();

    // Determin which letter should precede the coordinate
    let dir_letter = if degrees < 0 {
        if is_latitude {
            'S'
        } else {
            'W'
        }
    } else {
        if is_latitude {
            'N'
        } else {
            'E'
        }
    };

    // Return the string
    return format!("{}{:03}.{:02}.{:07.3}", dir_letter, degrees, minutes, seconds);
}

fn convert_vrc_to_decimal_deg_single(vrc_coord: &str) -> Option<Angle> {
    //W077.52.27.771
    let first = &vrc_coord[0..1];
    let sign = if first == "N" || first == "E" { 1 } else { -1 };
    if let Ok(mut degrees) = &vrc_coord[1..4].parse::<i32>() {
        degrees *= sign;
        if let Ok(minutes) = &vrc_coord[5..7].parse::<u32>() {
            if let Ok(seconds) = &vrc_coord[8..].parse::<f64>() {
                return Some(Angle::from_deg_min_sec(degrees, *minutes, *seconds));
            }
        }
    }
    return None;
}
