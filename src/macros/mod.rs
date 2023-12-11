#[doc(hidden)]
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

#[doc(hidden)]
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