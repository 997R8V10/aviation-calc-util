use std::time::Duration;

use crate::units::{Velocity, Acceleration, Length};

/// Calculates Final Velocity from Initial Velocity and Time
/// 
/// Kinematics Equation: V<sub>f</sub> = V<sub>i</sub> + a * t
pub fn kinematics_final_velocity(initial_velocity: Velocity, acceleration: Acceleration, time: Duration) -> Velocity {
    return initial_velocity + (acceleration * time);
}

/// Calculates Final Velocity from Initial Velocity and Time
/// 
/// Kinematics Equation: V<sub>f</sub> = V<sub>i</sub> + a * t
/// 
/// **Note**: All Units must be kept constant!
pub fn kinematics_final_velocity_general(initial_velocity: f64, acceleration: f64, time: f64) -> f64 {
    return initial_velocity + (acceleration * time);
}

/// Calculates Displacement from Initial Velocity, Final Velocity, and Time
/// 
/// Kinematics Equation: Δx = ((V<sub>i</sub> + V<sub>f</sub>)/2) * t
pub fn kinematics_displacement_1(initial_velocity: Velocity, final_velocity: Velocity, time: Duration) -> Length {
    return (initial_velocity + final_velocity) * time / 2.0;
}


/// Calculates Displacement from Initial Velocity, Final Velocity, and Time
/// 
/// Kinematics Equation: Δx = ((V<sub>i</sub> + V<sub>f</sub>)/2) * t
/// 
/// **Note**: All Units must be kept constant!
pub fn kinematics_displacement_1_general(initial_velocity: f64, final_velocity: f64, time: f64) -> f64 {
    return (initial_velocity + final_velocity) * time / 2.0;
}

/// Calculates Displacement from Initial Velocity, Acceleration, and Time
/// 
/// Kinematics Equation: Δx = V<sub>i</sub> * t + (1/2) * a * t<sup>2</sup>
pub fn kinematics_displacement_2(initial_velocity: Velocity, acceleration: Acceleration, time: Duration) -> Length {
    return initial_velocity * time + acceleration * time * time * 0.5;
}


/// Calculates Displacement from Initial Velocity, Acceleration, and Time
/// 
/// Kinematics Equation: Δx = V<sub>i</sub> * t + (1/2) * a * t<sup>2</sup>
/// 
/// **Note**: All Units must be kept constant!
pub fn kinematics_displacement_2_general(initial_velocity: f64, acceleration: f64, time: f64) -> f64 {
    return initial_velocity * time + acceleration * time * time * 0.5;
}

/// Calculates Time from Displacement, Initial Velocity, and Final Velocity.
/// 
/// Kinematics Equation: t = 2 * Δx / (V<sub>i</sub> + V<sub>f</sub>)
pub fn kinematics_time_1(displacement: Length, initial_velocity: Velocity, final_velocity: Velocity) -> Duration {
    return displacement * 2.0 / (initial_velocity + final_velocity);
}

/// Calculates Time from Displacement, Initial Velocity, and Final Velocity.
/// 
/// Kinematics Equation: t = 2 * Δx / (V<sub>i</sub> + V<sub>f</sub>)
/// 
/// **Note**: All Units must be kept constant!
pub fn kinematics_time_1_general(displacement: f64, initial_velocity: f64, final_velocity: f64) -> f64 {
    return displacement * 2.0 / (initial_velocity + final_velocity);
}

/// Calculates Acceleration from Initial Velocity, Final Velocity, and Time
/// 
/// Kinematics Equation = a = (V<sub>f</sub> - V<sub>i</sub>) / t
pub fn kinematics_acceleration(initial_velocity: Velocity, final_velocity: Velocity, time: Duration) -> Acceleration {
    return (final_velocity - initial_velocity) / time;
}

/// Calculates Acceleration from Initial Velocity, Final Velocity, and Time
/// 
/// Kinematics Equation = a = (V<sub>f</sub> - V<sub>i</sub>) / t
pub fn kinematics_acceleration_general(initial_velocity: f64, final_velocity: f64, time: f64) -> f64 {
    return (final_velocity - initial_velocity) / time;
}