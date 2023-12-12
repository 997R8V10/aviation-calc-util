use std::sync::{Mutex, Arc};

use chrono::NaiveDate;

use crate::{geo::{GeoTile, GeoTileBounds, GeoPoint, Bearing}, units::Angle};

use super::{MagneticModel, MagneticField};

/// A Thread-Safe Manager for Magnetic Conversions.
/// 
/// This increases performance by caching magnetic calculations
pub struct MagneticTileManager {
    tiles: Mutex<Vec<Arc<MagneticTile>>>,
    model: MagneticModel
}

impl MagneticTileManager {
    pub fn new(model: MagneticModel) -> MagneticTileManager {
        return MagneticTileManager { tiles: Mutex::new(Vec::new()), model };
    }

    pub fn find_or_create_tile(&self, point: &GeoPoint, date: &NaiveDate) -> Arc<MagneticTile> {
        // Get mutex guard
        let mut mutex_guard = self.tiles.lock().unwrap();

        // Look for tile
        for tile in mutex_guard.iter() {
            if tile.is_valid(point, date) {
                return Arc::clone(tile);
            }
        }

        // Create if not found
        let tile = Arc::new(MagneticTile::new(point, date, &self.model));
        mutex_guard.push(Arc::clone(&tile));

        return tile;
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
    fn bottom_lat(&self) -> crate::geo::Latitude {
        return self.bounds.bottom_lat();
    }

    fn top_lat(&self) -> crate::geo::Latitude {
        return self.bounds.top_lat();
    }

    fn left_lon(&self) -> crate::geo::Longitude {
        return self.bounds.left_lon();
    }

    fn right_lon(&self) -> crate::geo::Longitude {
        return self.bounds.right_lon();
    }
}

impl MagneticTile {
    pub const MAG_TILE_RES: Angle = Angle(0.1);
    
    pub fn new(point: &GeoPoint, date: &NaiveDate, model: &MagneticModel) -> MagneticTile {
        let bounds = GeoTileBounds::new_from_point(point, Self::MAG_TILE_RES);
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