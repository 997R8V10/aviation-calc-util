use std::{sync::{Mutex, Arc}, path::PathBuf};

use chrono::{DateTime, Utc};

use crate::geo::{GeoPoint, GeoTile};

use super::GribTile;

/// A Thread-Safe Manager for Grib Data access.
pub struct GribTileManager {
    tiles: Mutex<Vec<Arc<Mutex<GribTile>>>>,
    download_path: PathBuf,
}

impl GribTileManager {
    pub fn new(download_path: &PathBuf) -> GribTileManager {
        return GribTileManager {
            tiles: Mutex::new(Vec::new()),
            download_path: download_path.clone(),
        };
    }

    pub fn find_or_create_tile(&self, point: &GeoPoint, date: &DateTime<Utc>) -> Arc<Mutex<GribTile>> {
        // Get mutex guard
        let mut mutex_guard = self.tiles.lock().unwrap();

        // Look for tile
        for tile in mutex_guard.iter() {
            let tile_guard = tile.lock().unwrap();
            if tile_guard.is_valid(date) && tile_guard.is_point_in_tile(point) {
                return Arc::clone(tile);
            }
        }

        // Create if not found
        let tile = Arc::new(Mutex::new(GribTile::new(point, date, &self.download_path)));
        mutex_guard.push(Arc::clone(&tile));

        return tile;
    }
}