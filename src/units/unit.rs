pub trait Unit {
    fn new(val: f64) -> Self;

    fn value(&self) -> f64;
}