use chrono::NaiveDate;

use super::get_epoch_year;

#[derive(Clone, Copy, Default, PartialEq, Debug)]
pub struct MagneticModelCoefficients {
    pub n: usize,
    pub m: usize,
    pub g_nm: f64,
    pub h_nm: f64,
    pub g_dot_nm: f64,
    pub h_dot_nm: f64,
}

impl MagneticModelCoefficients {
    pub fn get_point_on_date(&self, model_epoch: f64, date: &NaiveDate) -> MagneticModelCoefficients {
        let date_model_epoch = get_epoch_year(date);
        let g_nm_t = self.g_nm + (date_model_epoch - model_epoch) * self.g_dot_nm;
        let h_nm_t = self.h_nm + (date_model_epoch - model_epoch) * self.h_dot_nm;

        return MagneticModelCoefficients {
            n: self.n,
            m: self.m,
            g_nm: g_nm_t,
            h_nm: h_nm_t,
            g_dot_nm: self.g_dot_nm,
            h_dot_nm: self.h_dot_nm,
        };
    }
}