mod polynomial;

pub use polynomial::Polynomial;

/// Legendre Polynomial helper utilities
pub mod legendre;

pub fn factorial(n: i32) -> i64 {
    let mut result: i64 = 1;

    if n <= 1 {
        return 1;
    }

    for i in 2..=n {
        result *= i as i64;
    }

    return result;
}

pub fn factorial_ratio(n: i32, m: i32) -> f64 {
    if (n > m) {
        return n as f64 * factorial_ratio(n - 1, m);
    }

    return 1.0;
}


/// Interoplates between two numbers with a multiplier.
/// 
/// # Examples
/// Find the number 30% of the way between 10 and 50
/// ```
/// use aviation_calc_util::math::interpolate_numbers;
///
/// let result = interpolate_numbers(10.0, 50.0, 0.3);
/// assert_eq!(result, 22.0);
/// ```
pub fn interpolate_numbers(start: f64, end: f64, multiplier: f64) -> f64 {
    return multiplier * (end - start) + start;
}

pub fn create_line_equation(x_1: f64, y_1: f64, x_2: f64, y_2: f64) -> Option<Polynomial> {
    // Check for Vertical Line
    if (x_2 - x_1 == 0.0){
        return None;
    }

    let m = (y_2 - y_1) / (x_2 - x_1);
    let b = y_1 - (m * x_1);

    return Some(Polynomial::new(&vec![b, m]));
}

pub fn find_2_lines_intersection(m_1: f64, b_1: f64, m_2: f64, b_2: f64) -> (f64, f64) {
    let no_m_1 = m_1.is_infinite() || m_1.is_nan() || b_1.is_nan();
    let no_m_2 = m_2.is_infinite() || m_2.is_nan() || b_2.is_nan();

    if (no_m_1 && no_m_2){
        return (0.0, 0.0);
    }

    if (no_m_1){
        return (0.0, b_2);
    }

    if (no_m_2){
        return (0.0, b_1);
    }

    if (m_2 - m_1 == 0.0){
        return (0.0, 0.0);
    }

    let x = (b_1 - b_2) / (m_2 - m_1);
    let y = m_1 * x + b_1;

    return (x, y);
}