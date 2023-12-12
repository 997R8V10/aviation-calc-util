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