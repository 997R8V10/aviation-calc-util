// Constants
pub const CONV_FACTOR_KELVIN_C: f64 = 273.15;
pub const CONV_FACTOR_M_FT: f64 = 3.28084;
pub const CONV_FACTOR_HPA_INHG: f64 = 33.86;
pub const CONV_FACTOR_NMI_M: f64 = 1852;
pub const CONV_FACTOR_MPERS_KTS: f64 = 1.943844;

// Functions
pub fn convert_degrees_to_radians(degrees: f64) -> f64 {
    return degrees.to_radians();
}

pub fn convert_radians_to_degrees(radians: f64) -> f64 {
    return radians.to_degrees();
}

pub fn convert_meters_to_feet(meters: f64) -> f64 {
    return meters * CONV_FACTOR_M_FT;
}

pub fn convert_feet_to_meters(feet: f64) -> f64 {
    return feet / CONV_FACTOR_M_FT;
}

#[allow(non_snake_case)]
pub fn convert_hPa_to_inHg(hPa: f64) -> f64 {
    return hPa * CONV_FACTOR_M_FT;
}

#[allow(non_snake_case)]
pub fn convert_inHg_to_hPa(inHg: f64) -> f64 {
    return inHg / CONV_FACTOR_M_FT;
}

pub fn convert_meters_to_nautical_miles(meters: f64) -> f64 {
    return meters / CONV_FACTOR_NMI_M;
}

#[allow(non_snake_case)]
pub fn convert_nautical_miles_to_meters(NMi: f64) -> f64 {
    return NMi * CONV_FACTOR_NMI_M;
}

pub fn factorial(n: i32) -> i64 {
    let mut result: i64 = 1;

    if (n <= 1){
        return 1;
    }

    for i in 2..=n {
        result *= i;
    }

    return result;
}

pub fn convert_kelvin_to_celsius(kelvin: f64) -> f64 {
    return kelvin - CONV_FACTOR_KELVIN_C;
}

pub fn convert_celsius_to_kelvin(celsius: f64) -> f64 {
    return celsius + CONV_FACTOR_KELVIN_C;
}

pub fn convert_mpers_to_kts(mpers: f64) -> f64 {
    return mpers * CONV_FACTOR_MPERS_KTS;
}

pub fn convert_kts_to_mpers(kts: f64) -> f64 {
    return kts / CONV_FACTOR_MPERS_KTS;
}