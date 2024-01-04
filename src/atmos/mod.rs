use std::f64::consts::E;

use crate::{
    geo::EARTH_GRAVITY,
    units::{Length, Pressure, Temperature, Velocity, Unit},
};

/// Atmospheric Grib Data
pub mod grib;

/// Specific Gas Constant for dry air (J/(kg*K))
pub const R_DRY_AIR: f64 = 287.058;

/// Ratio of specific heat at a constant pressure to heat at a constant volume for air
pub const SPEC_HEAT_RATIO_AIR: f64 = 1.4;

/// ISA Sea Level Standard Temperature
pub const ISA_STD_TEMP: Temperature = Temperature(288.15);

/// ISA Sea Level Standard Pressure
pub const ISA_STD_PRES: Pressure = Pressure(101325.0);

/// ISA Sea Level Air Density (kg/m^3)
pub const ISA_STD_DENS: f64 = ISA_STD_PRES.0 / (R_DRY_AIR * ISA_STD_TEMP.0);

/// ISA Standard Lapse Rate (K/m) below 11000m altitude.
pub const ISA_STD_LAPSE_RATE: f64 = 0.0065;

/// ISA Temperature in the Stratosphere (above 11000m)
pub const ISA_STRATO_TEMP: Temperature = Temperature(216.65);

/// Conversion Factor between Pressure and Altitude (m/Pa)
pub const ISA_STD_PRES_DROP_PER_ALT: f64 = 0.09144;

/// Calculates the density of dry air at a given temperature and pressure.
///
/// **Parameters:**
/// - `p`: Absolute Pressure
/// - `t`: Absolute Temperature
///
/// **Returns:**
/// - rho: Dry air density (kg/m^3)
///
/// **Reference:** <https://en.wikipedia.org/wiki/Density_of_air>
pub fn calculate_dry_air_density(p: Pressure, t: Temperature) -> f64 {
    return p.as_pascals() / (R_DRY_AIR * t.as_kelvin());
}

/// Calculates static air pressure at a given altitude
///
/// **Parameters:**
/// - `h`: Desired altitude above mean sea level
/// - `h_0`: Reference altitude above mean sea level
/// - `p_0`: Reference static air pressure
/// - `t`: Absolute temperature at `h`
///
/// **Returns:**
/// - p: Static air pressure at `h`
///
/// **Reference:** <https://www.omnicalculator.com/physics/air-pressure-at-altitude>
pub fn calculate_pressure_at_alt(h: Length, h_0: Length, p_0: Pressure, t: Temperature) -> Pressure {
    let e_pow = (-EARTH_GRAVITY.as_meters_per_second_squared() * (h - h_0).as_meters()) / (R_DRY_AIR * t.as_kelvin());

    return p_0 * E.powf(e_pow);
}

/// Calculates the absolute temperature at a given altitude
///
/// **Parameters:**
/// - `h`: Desired altitude above mean sea level
/// - `h_0`: Reference altitude above mean sea level
/// - `t_0`: Reference absolute temperature
///
/// **Returns:**
/// - T: Absolute temperature at `h`
pub fn calculate_temp_at_alt(h: Length, h_0: Length, t_0: Temperature) -> Temperature {
    return t_0 - ((h - h_0).as_meters() * ISA_STD_LAPSE_RATE);
}

/// Calculates the density altitude
///
/// **Parameters:**
/// - `p`: Absolute pressure
/// - `t`: Absolute temperature
///
/// **Returns:**
/// - Density altitude
///
/// **Reference:** <https://www.omnicalculator.com/physics/air-pressure-at-altitude>
pub fn calculate_density_altitude(p: Pressure, t: Temperature) -> Length {
    let pres_ratio = (p / ISA_STD_PRES).as_pascals();
    let temp_ratio = (t / ISA_STD_TEMP).as_kelvin();
    let exp = ((EARTH_GRAVITY.as_meters_per_second_squared() / (ISA_STD_LAPSE_RATE * R_DRY_AIR)) - 1.0).powi(-1);
    return Length::new((ISA_STD_TEMP.as_kelvin() / ISA_STD_LAPSE_RATE) * (1.0 - (pres_ratio / temp_ratio).powf(exp)));
}

/// Calculates the impact pressure
///
/// **Parameters:**
/// - `cas`: Calibrated airspeed
///
/// **Returns:**
/// - qc: Impact pressure
///
/// **Reference:** <https://en.wikipedia.org/wiki/Calibrated_airspeed>
pub fn calculate_impact_pressure_at_cas(cas: Velocity) -> Pressure {
    let a0 = calculate_speed_of_sound_dry_air(ISA_STD_TEMP);

    let coeff1 = 2.0 / (SPEC_HEAT_RATIO_AIR - 1.0);
    let coeff2 = SPEC_HEAT_RATIO_AIR / (SPEC_HEAT_RATIO_AIR - 1.0);

    let x1 = ((cas / a0).as_meters_per_second().powi(2) + coeff1) / coeff1;
    let x2 = x1.powf(coeff2) - 1.0;

    return ISA_STD_PRES * x2;
}

/// Calculates the calibrated air speed
///
/// **Parameters:**
/// - `qc`: Impact pressure
///
/// **Returns:**
/// - cas: Calibrated air speed
///
/// **Reference:** <https://en.wikipedia.org/wiki/Calibrated_airspeed>
pub fn calculate_calibrated_airspeed(qc: Pressure) -> Velocity {
    let a0 = calculate_speed_of_sound_dry_air(ISA_STD_TEMP);

    let coeff1 = 2.0 / (SPEC_HEAT_RATIO_AIR - 1.0);
    let coeff2 = (SPEC_HEAT_RATIO_AIR - 1.0) / SPEC_HEAT_RATIO_AIR;

    let x1 = ((qc / ISA_STD_PRES).as_pascals() + 1.0).powf(coeff2);
    let x2 = f64::sqrt(coeff1 * (x1 - 1.0));

    return a0 * x2;
}

/// Calculates the mach number
///
/// **Parameters:**
/// - `qc`: Impact pressure
/// - `p`: Absolute pressure
///
/// **Returns:**
/// - M: Mach number
///
/// **Reference:** <https://en.wikipedia.org/wiki/Mach_number>
pub fn calculate_mach_number(qc: Pressure, p: Pressure) -> f64 {
    let x1 = 2.0 / (SPEC_HEAT_RATIO_AIR - 1.0);
    let x2exp = (SPEC_HEAT_RATIO_AIR - 1.0) / SPEC_HEAT_RATIO_AIR;
    let x2 = (qc / p).as_pascals() + 1.0;

    return f64::sqrt(x1 * (x2.powf(x2exp) - 1.0));
}

/// Calculates the equivalent air speed
///
/// **Parameters:**
/// - `m`: Mach number
/// - `p`: Absolute pressure
///
/// **Returns:**
/// - eas: Equivalent air speed
///
/// **Reference:** <https://en.wikipedia.org/wiki/Equivalent_airspeed>
pub fn calculate_eas(m: f64, p: Pressure) -> Velocity {
    let a0 = calculate_speed_of_sound_dry_air(ISA_STD_TEMP);

    return a0 * m * f64::sqrt((p / ISA_STD_PRES).as_pascals());
}

/// Calculates the speed of sound in dry air at a given temperature
///
/// **Parameters:**
/// - `t`: Absolute temperature
///
/// **Returns:**
/// - Speed of sound
///
/// **Reference:** <https://en.wikipedia.org/wiki/Mach_number>
pub fn calculate_speed_of_sound_dry_air(t: Temperature) -> Velocity {
    return Velocity::new(f64::sqrt(SPEC_HEAT_RATIO_AIR * R_DRY_AIR * t.as_kelvin()));
}

/// Calculates the true air speed from mach number
///
/// **Parameters:**
/// - `m`: Mach number
/// - `t`: Absolute temperature
///
/// **Returns:**
/// - tas: True air speed
///
/// **Reference:** <https://en.wikipedia.org/wiki/True_airspeed>
pub fn convert_mach_to_tas(m: f64, t: Temperature) -> Velocity {
    let a0 = calculate_speed_of_sound_dry_air(ISA_STD_TEMP);

    return a0 * m * f64::sqrt((t / ISA_STD_TEMP).as_kelvin());
}

/// Calculates the impact pressure
///
/// **Parameters:**
/// - `m`: Mach number
/// - `p`: Absolute pressure
///
/// **Returns:**
/// - qc: Impact pressure
///
/// **Reference:** <https://en.wikipedia.org/wiki/Impact_pressure>
pub fn calculate_impact_pressure(m: f64, p: Pressure) -> Pressure {
    let x1 = (SPEC_HEAT_RATIO_AIR - 1.0) / 2.0;
    let x2exp = SPEC_HEAT_RATIO_AIR / (SPEC_HEAT_RATIO_AIR - 1.0);
    let x2 = (1.0 + x1 * m.powi(2));

    return p * (x2.powf(x2exp) - 1.0);
}

/// Calculates the mach number from the true air speed.
///
/// **Parameters:**
/// - `tas`: True air speed
/// - `t`: Absolute temperature
///
/// **Returns:**
/// - M: Mach number
///
/// **Reference:** <https://en.wikipedia.org/wiki/True_airspeed>
pub fn convert_tas_to_mach(tas: Velocity, t: Temperature) -> f64 {
    let mut t_kelvin = t.as_kelvin();

    // Prevent div by 0
    if (t_kelvin < 0.0) {
        t_kelvin = 1.0;
    }

    let a0 = calculate_speed_of_sound_dry_air(ISA_STD_TEMP);

    return tas.as_meters_per_second() / (a0 * f64::sqrt(t_kelvin / ISA_STD_TEMP.as_kelvin())).as_meters_per_second();
}

/// Converts Indicated Air Speed to True Air Speed
///
/// **Parameters:**
/// - `ias`: Indicated air speed
/// - `ref_press`: Reference pressure
/// - `alt`: True altitude
/// - `ref_alt`: Reference altitude
/// - `ref_temp`: Reference temperature
///
/// **Returns:**
/// - True air speed
/// - Mach Number
pub fn convert_ias_to_tas(ias: Velocity, ref_press: Pressure, alt: Length, ref_alt: Length, ref_temp: Temperature) -> (Velocity, f64) {
    // Calculate pressure and temp at height
    let t = calculate_temp_at_alt(alt, ref_alt, ref_temp);
    let p = calculate_pressure_at_alt(alt, ref_alt, ref_press, t);

    // Calculate impact (dynamic) pressure
    let qc = calculate_impact_pressure_at_cas(ias);

    // Calculate mach number
    let mach = calculate_mach_number(qc, p);

    // Calculate tas
    let tas = convert_mach_to_tas(mach, t);

    return (tas, mach);
}

/// Converts True Air Speed to Indicated Air Speed
///
/// **Parameters:**
/// - `tas`: True air speed
/// - `ref_press`: Reference pressure
/// - `alt`: True altitude
/// - `ref_alt`: Reference altitude
/// - `ref_temp`: Reference temperature
///
/// **Returns:**
/// - Indicated air speed
/// - Mach Number
pub fn convert_tas_to_ias(tas: Velocity, ref_press: Pressure, alt: Length, ref_alt: Length, ref_temp: Temperature) -> (Velocity, f64) {
    // Calculate pressure and temp at height
    let t = calculate_temp_at_alt(alt, ref_alt, ref_temp);
    let p = calculate_pressure_at_alt(alt, ref_alt, ref_press, t);

    // Calculate mach number
    let mach = convert_tas_to_mach(tas, t);

    // Calculate impact (dynamic) pressure
    let qc = calculate_impact_pressure(mach, p);

    // Calculate tas
    let cas = calculate_calibrated_airspeed(qc);

    return (cas, mach);
}

/// Converts Indicated Altitude to Absolute Altitude
///
/// **Parameters:**
/// - `alt_ind`: Indicated altitude
/// - `pres_set`: Pressure setting
/// - `sfc_pres`: Surface pressure
///
/// **Returns:**
/// - Absolute altitude
pub fn convert_indicated_to_absolute_alt(alt_ind: Length, pres_set: Pressure, sfc_pres: Pressure) -> Length {
    let press_diff = pres_set - sfc_pres;
    return alt_ind - (ISA_STD_PRES_DROP_PER_ALT * press_diff.as_pascals());
}

/// Converts Absolute Altitude to Indicated Altitude
///
/// **Parameters:**
/// - `alt_abs`: Absolute altitude
/// - `pres_set`: Pressure setting
/// - `sfc_pres`: Surface pressure
///
/// **Returns:**
/// - Indicated altitude
pub fn convert_absolute_to_indicated_alt(alt_abs: Length, pres_set: Pressure, sfc_pres: Pressure) -> Length {
    let press_diff = pres_set - sfc_pres;
    return alt_abs + (ISA_STD_PRES_DROP_PER_ALT * press_diff.as_pascals());
}

/// Converts Indicated Altitude to Pressure Altitude
///
/// **Parameters:**
/// - alt_ind`: Indicated altitude
/// - pres_set`: Pressure setting
///
/// **Returns:**
/// - Pressure altitude (ft)
pub fn convert_indicated_to_pressure_alt(alt_ind: Length, pres_set: Pressure) -> Length {
    let press_diff = pres_set - ISA_STD_PRES;
    return alt_ind - (ISA_STD_PRES_DROP_PER_ALT * press_diff.as_pascals());
}

/// Calculate ISA (International Standard Atmosphere) Temperature at a Pressure Altitude.
///
/// **Parameters:**
/// - alt_pres_ft Pressure altitude
///
/// **Returns:**
/// - ISA Temp
pub fn calculate_isa_temp(alt_pres: Length) -> Temperature {
    if (alt_pres >= Length::new(11_000.0)) {
        return ISA_STRATO_TEMP;
    }

    return ISA_STD_TEMP - (alt_pres.as_meters() * ISA_STD_PRES_DROP_PER_ALT);
}
