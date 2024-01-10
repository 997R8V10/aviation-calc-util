use crate::math::factorial;
use std::{collections::HashMap, sync::Mutex};

use super::{polynomial::Polynomial, factorial_ratio};

pub struct LegendreManager {
    legendre_cache: Mutex<HashMap<(i32, i32), Polynomial>>,
}

impl LegendreManager {
    pub fn new() -> LegendreManager {
        return LegendreManager {
            legendre_cache: Mutex::new(HashMap::new()),
        };
    }

    pub fn legendre_function(&self, n: i32, m: i32, x: f64) -> f64 {
        let key = (n, m);

        let mut cache = self.legendre_cache.lock().unwrap();
        let q;
        let p = if let Some(cache_value) = cache.get(&key) {
            cache_value
        } else {
            q = legendre_polynomial(n).derivative(m);

            cache.insert(key, q.clone());

            &q
        };

        return (1.0 - x.powi(2)).powf(m as f64 / 2.0) * p.evaluate(x);
    }
}

pub fn legendre_polynomial(n: i32) -> Polynomial {
    let mut coeffs = vec![0.0; n as usize + 1];

    let mut m = 0;

    while (m <= n / 2) {
        let mut val = f64::powi(-1.0, m) / 2.0_f64.powi(n);
        val *= factorial_ratio(2 * n - 2 * m, n - m) / (factorial(m) * factorial(n - 2 * m)) as f64;
        coeffs.insert((n - 2 * m) as usize, val);

        m += 1;
    }

    return Polynomial::new(&coeffs);
}
