use std::sync::Mutex;

use chrono::NaiveDate;

use crate::{geo::{geo_tile::{GeoTile, GeoTileBounds}, geo_point::GeoPoint, bearing::Bearing}, units::angle::Angle};

use super::magnetic_model::{MagneticField, MagneticModel};

pub const MAG_TILE_RES: Angle = Angle(0.1);

/// A Thread-Safe Manager for Magnetic Conversions.
/// 
/// This increases performance by caching magnetic calculations
pub struct MagneticTileManager {
    tiles: Mutex<Vec<MagneticTile>>,
    model: MagneticModel
}

impl MagneticTileManager {
    pub fn new(model: MagneticModel) -> MagneticTileManager {
        return MagneticTileManager { tiles: Mutex::new(Vec::new()), model: model };
    }

    pub fn find_or_create_tile(&self, point: &GeoPoint, date: &NaiveDate) -> MagneticTile {
        // Get mutex guard
        let mut mutex_guard = self.tiles.lock().unwrap();

        // Look for tile
        for (_i, tile) in mutex_guard.iter().enumerate() {
            if tile.is_valid(point, date) {
                return tile.clone();
            }
        }

        // Create if not found
        let tile = MagneticTile::new(point, date, &self.model);
        mutex_guard.push(tile);

        return tile.clone();
    }

    pub fn true_to_magnetic(&self, point: &GeoPoint, date: &NaiveDate, true_bearing: Bearing) -> Bearing {
        let tile = self.find_or_create_tile(point, date);

        return tile.field.true_to_magnetic(true_bearing);
    }

    pub fn magnetic_to_true(&self, point: &GeoPoint, date: &NaiveDate, magnetic_bearing: Bearing) -> Bearing {
        let tile = self.find_or_create_tile(point, date);
        
        return tile.field.magnetic_to_true(magnetic_bearing);
    }
}

#[derive(Clone, Copy, Default, PartialEq, Debug)]
pub struct MagneticTile {
    pub bounds: GeoTileBounds,
    pub date: NaiveDate,
    pub field: MagneticField
}

impl GeoTile for MagneticTile {
    fn bottom_lat(&self) -> crate::geo::latitude::Latitude {
        return self.bounds.bottom_lat();
    }

    fn top_lat(&self) -> crate::geo::latitude::Latitude {
        return self.bounds.top_lat();
    }

    fn left_lon(&self) -> crate::geo::longitude::Longitude {
        return self.bounds.left_lon();
    }

    fn right_lon(&self) -> crate::geo::longitude::Longitude {
        return self.bounds.right_lon();
    }
}

impl MagneticTile {
    pub fn new(point: &GeoPoint, date: &NaiveDate, model: &MagneticModel) -> MagneticTile {
        let bounds = GeoTileBounds::new_from_point(point, MAG_TILE_RES);
        return MagneticTile {
            bounds: bounds,
            date: date.clone(),
            field: model.calculate_field(&bounds.center_point(), date)
        };
    }

    pub fn is_valid(&self, point: &GeoPoint, date: &NaiveDate) -> bool {
        return self.is_point_in_tile(point) && self.date == *date;
    }
 }