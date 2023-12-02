use time::Date;

#[derive(Clone, Copy, Default, PartialEq, Debug)]
pub struct MagneticModelCoefficients {
    pub n: i32,
    pub m: i32,
    pub g_nm: f64,
    pub h_nm: f64,
    pub g_dot_nm: f64,
    pub h_dot_nm: f64,
}

impl MagneticModelCoefficients {
    pub fn get_point_on_date(&self, model_epoch: f64, date: &Date) -> MagneticModelCoefficients {
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

pub fn get_epoch_year(date: &Date) -> f64 {
    let (year, day_of_year) = date.to_ordinal_date();

    let mut days_percentage = (day_of_year - 1) as f64 / 365.0;
    if let Ok(next_year_start) = Date::from_ordinal_date(year + 1, 1) {
        if let Ok(this_year_start) = Date::from_ordinal_date(year, 1) {
            let total_days = (next_year_start - this_year_start).whole_days();
            days_percentage = (day_of_year - 1) as f64 / total_days as f64;
        }
    }

    return year as f64 + days_percentage;
}
