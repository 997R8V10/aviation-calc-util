pub trait Unit {
    fn new(val: f64) -> Self;

    fn value(&self) -> f64;

    fn abs(self) -> Self where Self: Sized {
        return Self::new(self.value().abs());
    }

    fn powi(self, n: i32) -> Self where Self: Sized {
        return Self::new(self.value().powi(n));
    }

    fn powf(self, n: f64) -> Self where Self: Sized {
        return Self::new(self.value().powf(n));
    }

    fn sqrt(self) ->Self where Self: Sized {
        return Self::new(self.value().sqrt());
    }

    fn sin(self) ->Self where Self: Sized {
        return Self::new(self.value().sin());
    }

    fn cos(self) ->Self where Self: Sized {
        return Self::new(self.value().cos());
    }

    fn tan(self) ->Self where Self: Sized {
        return Self::new(self.value().tan());
    }

    fn sinh(self) ->Self where Self: Sized {
        return Self::new(self.value().sinh());
    }

    fn cosh(self) ->Self where Self: Sized {
        return Self::new(self.value().cosh());
    }

    fn tanh(self) ->Self where Self: Sized {
        return Self::new(self.value().tanh());
    }

    fn asin(self) ->Self where Self: Sized {
        return Self::new(self.value().asin());
    }

    fn acos(self) ->Self where Self: Sized {
        return Self::new(self.value().acos());
    }

    fn atan(self) ->Self where Self: Sized {
        return Self::new(self.value().atan());
    }

    fn atan2(self, other: &Self) ->Self where Self: Sized {
        return Self::new(self.value().atan2(other.value()));
    }
}