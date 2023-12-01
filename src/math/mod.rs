pub mod polynomial;
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