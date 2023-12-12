
/// Representaion of a polynomial
#[derive(Clone, Default, PartialEq, Debug)]
pub struct Polynomial{
    coefficients: Vec<f64>
}

impl Polynomial {
    pub fn new(coefficients: &Vec<f64>) -> Polynomial {
        return Polynomial { coefficients: coefficients.clone() }
    }

    pub fn evaluate(&self, x: f64) -> f64 {
        let mut y = 0.0;
        for (i, coeff) in self.coefficients.iter().enumerate() {
            y += coeff * x.powi(i as i32);
        }

        return y;
    }

    pub fn derivative(&self, n: i32) -> Polynomial {
        if n == 1 {
            let mut d_coeffs = Vec::with_capacity(self.coefficients.len() - 1);

            for (i, coeff) in self.coefficients.iter().enumerate() {
                if (i == 0) {
                    continue;
                }

                d_coeffs.push(i as f64 * coeff);
            }

            return Polynomial::new(&d_coeffs);
        }

        let mut q = Polynomial::new(&self.coefficients);

        for _i in 0..n {
            q = q.derivative(1);
        }

        return q;
    }

    pub fn coefficients(&self) -> Vec<f64> {
        return self.coefficients.clone();
    }
}