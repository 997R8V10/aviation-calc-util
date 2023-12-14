use std::{f64::consts::FRAC_PI_2, f64::consts::PI};

use crate::{
    geo::{
        Bearing,
        GeoPoint,
        EARTH_RADIUS, EARTH_GRAVITY,
    },
    units::{Angle, AngularVelocity, Length, Velocity, Unit},
};

#[derive(Clone, Copy, Default, PartialEq, Debug)]
pub struct CourseInterceptInfo {
    pub required_course: Bearing,
    pub along_track_distance: Length,
    pub cross_track_error: Length,
}

// Display
impl std::fmt::Display for CourseInterceptInfo {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        return write!(
            f,
            "crs: {}°, aTk: {}, xTk: {}",
            self.required_course.as_degrees(),
            self.along_track_distance,
            self.cross_track_error
        );
    }
}

/// Calculates a maximum bank angle for a given rate of turn, ground speed, and bank angle limit.
///
/// # Examples
///
/// ```
/// use aviation_calc_util::aviation::calculate_max_bank_angle;
/// use aviation_calc_util::units::{Angle, AngularVelocity, Velocity};
///
/// let result = calculate_max_bank_angle(Velocity::from_knots(250.0), Angle::from_degrees(25.0), AngularVelocity::from_degrees_per_second(3.0));
/// assert_eq!(result, Angle::from_degrees(25.0));
/// ```
pub fn calculate_max_bank_angle(ground_speed: Velocity, bank_limit: Angle, turn_rate: AngularVelocity) -> Angle {
    // bank angle = atan(rate of turn * v / g)
    let bank_angle = Angle::new(f64::atan2(
        turn_rate.as_radians_per_second() * ground_speed.as_meters_per_second(),
        EARTH_GRAVITY,
    ));

    return if bank_angle > bank_limit { bank_limit } else { bank_angle };
}

/// Calculates radius of turn at a bank angle and ground speed.
///
/// # Examples
///
/// ```
/// use aviation_calc_util::aviation::calculate_radius_of_turn;
/// use aviation_calc_util::units::{Angle, Velocity, Length, Unit};
///
/// let calculated = calculate_radius_of_turn(Velocity::from_knots(250.0), Angle::from_degrees(25.0));
/// let expected = Length::new(3616.0);
///
/// let abs_difference = (calculated - expected).abs().as_meters();
/// assert!(abs_difference <= 10.0);
/// ```
pub fn calculate_radius_of_turn(ground_speed: Velocity, bank_angle: Angle) -> Length {
    if (ground_speed == Velocity(0.0)) {
        return Length::new(1.0);
    }

    // R = V^2 / (g * tan(bank_angle))
    return Length::new(ground_speed.powi(2).as_meters_per_second() / (EARTH_GRAVITY * f64::tan(bank_angle.as_radians())));
}

/// Calculates bank angle at a certain radius of turn and ground_speed
pub fn calculate_bank_angle(radius_of_turn: Length, ground_speed: Velocity) -> Angle {
    return Angle::new(f64::atan2(
        ground_speed.powi(2).as_meters_per_second(),
        EARTH_GRAVITY * radius_of_turn.as_meters(),
    ));
}

/// Calculates the minimum turn radius for a particular turn.
///
/// Factors in wind and makes sure the bank will remain below the bank limit.
pub fn calculate_constant_radius_turn(
    start_bearing: Bearing,
    turn_amount: Angle,
    wind_bearing: Bearing,
    wind_speed: Velocity,
    true_airspeed: Velocity,
    bank_limit: Angle,
    turn_rate: AngularVelocity,
) -> Length {
    // Get ground speed at max wind
    let max_gs = calculate_max_ground_speed_for_turn(start_bearing, turn_amount, wind_bearing, wind_speed, true_airspeed);

    // Return minimum radius
    return calculate_radius_of_turn(max_gs, calculate_max_bank_angle(max_gs, bank_limit, turn_rate));
}

/// Calculates the maximum ground speed that will be achieved in a turn.
pub fn calculate_max_ground_speed_for_turn(
    start_bearing: Bearing,
    turn_amount: Angle,
    wind_bearing: Bearing,
    wind_speed: Velocity,
    true_airspeed: Velocity,
) -> Velocity {
    // Find bearing with maximum tailwind
    let tail_wind_direction = -wind_bearing;
    let mut wind_turn_amt = (tail_wind_direction - start_bearing).as_radians();
    let turn_amt = turn_amount.as_radians();

    // Adjust wind turn amount to match turn amount
    if (turn_amt > 0.0 && wind_turn_amt < 0.0) {
        wind_turn_amt += 2.0 * PI;
    } else if (turn_amt < 0.0 && wind_turn_amt > 0.0) {
        wind_turn_amt -= 2.0 * PI;
    }

    let mut max_wind_bearing = wind_bearing;

    // Check if wind falls outside bounds
    if (wind_turn_amt > turn_amt && turn_amt > 0.0) {
        let half_boundary = turn_amt + (2.0 * PI - 0.5 * turn_amt);
        max_wind_bearing = if wind_turn_amt <= half_boundary {
            -(start_bearing + turn_amount)
        } else {
            -start_bearing
        };
    } else if (wind_turn_amt < turn_amt && turn_amt < 0.0) {
        let half_boundary = turn_amt - (-2.0 * PI + 0.5 * turn_amt);
        max_wind_bearing = if wind_turn_amt >= half_boundary {
            -(start_bearing + turn_amount)
        } else {
            -start_bearing
        }
    }

    // Get ground speed at max wind
    return true_airspeed + get_headwind_component(wind_bearing, wind_speed, max_wind_bearing);
}

/// Get the headwind component.
///
/// Tailwind will be returned as negative
pub fn get_headwind_component(wind_bearing: Bearing, wind_speed: Velocity, bearing: Bearing) -> Velocity {
    return Velocity::new(wind_speed.as_meters_per_second() * f64::cos(bearing.as_radians() - wind_bearing.as_radians()));
}

/// Calculates the chord line bearing and distance for a turn.
pub fn calculate_chord_for_turn(start_bearing: Bearing, turn_amount: Angle, radius_of_turn: Length) -> (Bearing, Length) {
    let half_turn: Angle = turn_amount / 2.0;
    let chord_length = Length::new(2.0 * radius_of_turn.as_meters() * f64::sin(half_turn.as_radians()));
    let chord_bearing = start_bearing + half_turn;

    return (chord_bearing, chord_length);
}

/// Calculates the bearing to end point after a turn from start point.
///
/// i.e. An aircraft turning direct a waypoint
///
/// Returns an `Option<Bearing>`. If a bearing cannot be calculated, `None` is returned.
pub fn calculate_direct_bearing_after_turn(
    start_point: &GeoPoint,
    end_point: &GeoPoint,
    radius_of_turn: Length,
    start_bearing: Bearing,
) -> Option<Bearing> {
    // Set waypoint's altitude to aircraft's altitude to minimize error
    let temp_end_point = GeoPoint::new(end_point.lat, end_point.lon, start_point.alt);

    // Get direct bearing to waypoint from aircraft. Use this to figure out right or left turn.
    let is_right_turn = (GeoPoint::initial_bearing(&start_point, &end_point) - start_bearing).as_radians() > 0.0;

    // Calculate bearing to center point
    let bearing_to_center = if is_right_turn {
        start_bearing + Angle::new(FRAC_PI_2)
    } else {
        start_bearing - Angle::new(FRAC_PI_2)
    };

    // Find center point
    let mut center = start_point.clone();
    center.move_by(bearing_to_center, radius_of_turn);

    // Find distance and bearing from center to end point
    let final_bearing_c = GeoPoint::final_bearing(&center, &temp_end_point);
    let distance_c = GeoPoint::distance(&center, &temp_end_point);

    // Find angle between final_bearing_c and desired bearing
    let ang = f64::asin(radius_of_turn.as_meters() / distance_c.as_meters());

    // Calculate final bearing to end point
    let turn_dir_bearing = if is_right_turn {
        final_bearing_c.as_radians() + ang
    } else {
        final_bearing_c.as_radians() - ang
    };

    if turn_dir_bearing.is_nan() {
        return None;
    }

    return Some(Bearing::from_radians(turn_dir_bearing));
}

/// Calculates the course intercept information for a linear course to a waypoint.
///
/// **Returns**:
/// - `required_course`: Required course along great circle path at current position.
/// - `along_track_distance`: Distance along course great circle path. Negative if station passage has occured.
/// - `cross_track_error`: Lateral offset from course. Right: Positive; Left: Negative;
pub fn calculate_linear_course_intercept(aircraft: &GeoPoint, waypoint: &GeoPoint, course: Bearing) -> CourseInterceptInfo {
    // Set waypoint's altitude to aircraft's altitude to minimize error
    let temp_wp = GeoPoint::new(waypoint.lat, waypoint.lon, aircraft.alt);

    // Find radial
    let final_dir_bearing = GeoPoint::final_bearing(aircraft, &temp_wp);
    let dist = GeoPoint::flat_distance(aircraft, &temp_wp);

    let turn_amt = final_dir_bearing - course;

    let radial = if turn_amt.abs().as_radians() < FRAC_PI_2 { -course } else { course };

    // Calculate radius
    let r = (EARTH_RADIUS + aircraft.alt).as_meters();

    // Calculate angular distance between aircraft and waypoint
    let sigma_1_3 = dist.as_meters() / r;

    // Initial bearing from waypoint to aircraft
    let theta_1_3 = (-final_dir_bearing).as_radians();

    // Radial in radians
    let theta_1_2 = radial.as_radians();

    // Cross track error
    let x_track_m = f64::asin(f64::sin(sigma_1_3) * f64::sin(theta_1_3 - theta_1_2)) * r;

    // Along track distance
    let a_track_m = f64::acos(f64::cos(sigma_1_3) / f64::cos(x_track_m / r)) * r;

    // Find along track waypoint
    let mut a_track_point = temp_wp.clone();
    a_track_point.move_by(radial, Length::new(a_track_m));

    // Calculate required course
    let initial_bearing = GeoPoint::initial_bearing(&a_track_point, &temp_wp);
    let required_course = if turn_amt.abs().as_radians() < FRAC_PI_2 {
        initial_bearing
    } else {
        -initial_bearing
    };

    let along_track_distance = if turn_amt.abs().as_radians() < FRAC_PI_2 {
        Length::new(a_track_m)
    } else {
        Length::new(-a_track_m)
    };

    let cross_track_error = if turn_amt.abs().as_radians() < FRAC_PI_2 {
        Length::new(-x_track_m)
    } else {
        Length::new(x_track_m)
    };

    return CourseInterceptInfo {
        required_course,
        along_track_distance,
        cross_track_error,
    };
}

/// Calculates the course intercept information for an arc course.
///
/// **Returns**:
/// - `required_course`: Required course along great circle path at current position.
/// - `along_track_distance`: Distance along course great circle path. Negative if station passage has occured.
/// - `cross_track_error`: Lateral offset from course. Right: Positive; Left: Negative;
pub fn calculate_arc_course_intercept(
    aircraft: &GeoPoint,
    arc_center: &GeoPoint,
    start_radial: Bearing,
    end_radial: Bearing,
    radius: Length,
    is_clockwise: bool,
) -> CourseInterceptInfo {
    let cur_radial = GeoPoint::initial_bearing(arc_center, aircraft);

    // Create final bearing waypoints
    let mut cur_point = GeoPoint::new(arc_center.lat, arc_center.lon, Length::new(0.0));
    cur_point.move_by(cur_radial, radius);

    // Calculate final radials
    let cur_final_radial = GeoPoint::final_bearing(arc_center, &cur_point);

    // Calculate required course
    let required_course = if is_clockwise {
        cur_final_radial + Angle::new(FRAC_PI_2)
    } else {
        cur_final_radial - Angle::new(FRAC_PI_2)
    };

    // Calculate cross track error
    let cur_radius = GeoPoint::flat_distance(arc_center, aircraft);
    let cross_track_error = if is_clockwise { radius - cur_radius } else { cur_radius - radius };

    // Calculate along track distance
    let delta_start_to_end_radial = Bearing::calculate_bearing_delta_with_dir(start_radial, end_radial, is_clockwise);
    let delta_ppos_to_end_radial = Bearing::calculate_bearing_delta_with_dir(cur_radial, end_radial, is_clockwise);

    // Calculate crossover amount to divide up the rest of the circle
    let min_distance = delta_start_to_end_radial + Angle::new(0.5 * (2.0 * PI - delta_start_to_end_radial.as_radians()));

    // If end radial has been passed
    let along_track_distance = if delta_ppos_to_end_radial > min_distance {
        Length::new(-1.0)
    } else {
        // Calculate arc length
        Length::new(delta_ppos_to_end_radial.as_radians() * radius.as_meters())
        // theta * r
    };

    return CourseInterceptInfo {
        required_course,
        along_track_distance,
        cross_track_error,
    };
}

/// Calculates the distance between the intersecion of two tangent lines of a circle and the tangent point of the circle.
pub fn calculate_arc_tangent_distance(theta: Angle, r: Length) -> Length {
    let half_angle = theta.as_radians() / 2.0;

    let half_tan = f64::tan(half_angle);

    // Find lead in distance
    return r * Length::new(half_tan);
}

/// Calculates turn lead distance and some other parameters
/// 
/// **Returns:**
/// - Turn Lead Distance
/// - Radius of Turn
/// - Intersection
pub fn calculate_turn_lead_distance(
    pos: &GeoPoint,
    wp: &GeoPoint,
    cur_bearing: Bearing,
    true_airspeed: Velocity,
    course: Bearing,
    wind_direction: Bearing,
    wind_speed: Velocity,
    bank_limit: Angle,
    turn_rate: AngularVelocity,
) -> Option<(Length, Length, GeoPoint)> {
    // Find intersection
    let intersection = find_intersection_to_course(pos, wp, cur_bearing, course);

    if let Some(intersection) = intersection {
        // Find degrees to turn
        let turn_amt = course - cur_bearing;
        let theta = turn_amt.abs();

        // Calculate constant radius of turn
        let r = calculate_constant_radius_turn(cur_bearing, turn_amt, wind_direction, wind_speed, true_airspeed, bank_limit, turn_rate);

        let turn_lead_dist = calculate_arc_tangent_distance(theta, r);

        return Some((turn_lead_dist, r, intersection));
    } else {
        return None;
    }
}

/// Finds intersection between an aircraft and a course to/from a waypoint
///
/// Will try both radials of the waypoint ONLY.
pub fn find_intersection_to_course(position: &GeoPoint, wp: &GeoPoint, cur_bearing: Bearing, course: Bearing) -> Option<GeoPoint> {
    // Try both radials of wp and see which one works
    let int_1 = GeoPoint::intersection(position, cur_bearing, wp, course);
    let int_2 = GeoPoint::intersection(position, cur_bearing, wp, -course);

    if (int_1.is_none()) {
        return int_2;
    }

    if (int_2.is_none()) {
        return int_1;
    }

    let dist_1 = GeoPoint::flat_distance(position, &int_1.unwrap());
    let dist_2 = GeoPoint::flat_distance(position, &int_2.unwrap());

    if (dist_1 < dist_2) {
        return int_1;
    }

    return int_2;
}
