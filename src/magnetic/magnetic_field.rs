use crate::{units::Angle, geo::Bearing};

#[derive(Clone, Copy, Default, PartialEq, Debug)]
pub struct MagneticFieldElements {
    pub x: f64,
    pub y: f64,
    pub z: f64,
    pub h: f64,
    pub f: f64,
    pub incl: Angle,
    pub decl: Angle,
}

#[derive(Clone, Copy, Default, PartialEq, Debug)]
pub struct MagneticField {
    pub field_elements: MagneticFieldElements,
    pub sec_elements: MagneticFieldElements,
}

impl MagneticField {
    pub fn true_to_magnetic(&self, true_bearing: Bearing) -> Bearing {
        return true_bearing - self.field_elements.decl;
    }

    pub fn magnetic_to_true(&self, magnetic_bearing: Bearing) -> Bearing {
        return magnetic_bearing + self.field_elements.decl;
    }
}