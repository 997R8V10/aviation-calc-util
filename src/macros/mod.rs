
#[macro_export]
macro_rules! impl_one_part_op_for_number {
    ($number_type: ident, $op_type: ident, $method: ident, $opr: tt) => {
        impl $op_type for $number_type {
            type Output = $number_type;

            fn $method(self) -> Self::Output {
                return $number_type($opr self.0);
            }
        }
    }
}

#[macro_export]
macro_rules! impl_two_part_op_for_number {
    ($number_type: ident, $op_type: ident, $method: ident, $opr: tt) => {
        impl $op_type for $number_type {
            type Output = $number_type;
        
            fn $method(self, rhs: Self) -> Self::Output {
                return $number_type(self.0 $opr rhs.0);
            }
        }

        impl $op_type<f64> for $number_type {
            type Output = $number_type;

            fn $method(self, rhs: f64) -> Self::Output {
                return $number_type(self.0 $opr rhs);
            }
        }

        impl $op_type<f32> for $number_type {
            type Output = $number_type;

            fn $method(self, rhs: f32) -> Self::Output {
                return $number_type(self.0 $opr rhs as f64);
            }
        }

        impl $op_type<i32> for $number_type {
            type Output = $number_type;

            fn $method(self, rhs: i32) -> Self::Output {
                return $number_type(self.0 $opr rhs as f64);
            }
        }

        impl $op_type<i64> for $number_type {
            type Output = $number_type;

            fn $method(self, rhs: i64) -> Self::Output {
                return $number_type(self.0 $opr rhs as f64);
            }
        }
    };
}

#[macro_export]
macro_rules! impl_assign_op_for_number {
    ($number_type: ident, $op_type: ident, $method: ident, $opr: tt) => {
        impl $op_type for $number_type {        
            fn $method(&mut self, rhs: Self) {
                self.0 $opr rhs.0;
            }
        }

        impl $op_type<f64> for $number_type {
            fn $method(&mut self, rhs: f64) {
                self.0 $opr rhs as f64;
            }
        }

        impl $op_type<f32> for $number_type {
            fn $method(&mut self, rhs: f32) {
                self.0 $opr rhs as f64;
            }
        }

        impl $op_type<i32> for $number_type {
            fn $method(&mut self, rhs: i32) {
                self.0 $opr rhs as f64;
            }
        }

        impl $op_type<i64> for $number_type {
            fn $method(&mut self, rhs: i64) {
                self.0 $opr rhs as f64;
            }
        }
    };
}

#[macro_export]
macro_rules! impl_num_methods {
    () => {
        pub fn abs(self) -> Self {
            return Self(self.0.abs());
        }

        pub fn powi(self, n: i32) -> Self {
            return Self(self.0.powi(n));
        }

        pub fn powf(self, n: f64) -> Self {
            return Self(self.0.powf(n));
        }

        pub fn sqrt(self) -> Self {
            return Self(self.0.sqrt());
        }

        pub fn sin(self) -> Self {
            return Self(self.0.sin());
        }

        pub fn cos(self) -> Self {
            return Self(self.0.cos());
        }

        pub fn tan(self) -> Self {
            return Self(self.0.tan());
        }

        pub fn sinh(self) -> Self {
            return Self(self.0.sinh());
        }

        pub fn cosh(self) -> Self {
            return Self(self.0.cosh());
        }

        pub fn tanh(self) -> Self {
            return Self(self.0.tanh());
        }

        pub fn asin(self) -> Self {
            return Self(self.0.asin());
        }

        pub fn acos(self) -> Self {
            return Self(self.0.acos());
        }

        pub fn atan(self) -> Self {
            return Self(self.0.atan());
        }

        pub fn atan2(self, other: Self) -> Self {
            return Self(self.0.atan2(other.0));
        }
    }
}