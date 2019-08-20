#ifndef MAP_GFX_H
#define MAP_GFX_H

#include <memory>

#include "map/hexagons.h"

class MapGfx {
   public:
   private:
};

pub struct Map<'a> {
    pub layout: Rc<RefCell<hexagons::Layout>>,
    pub hexes: HashMap<hexagons::HexCoordinates, HexShape<'a>>,
    pub rivers: Vec<RiverShape<'a>>,
}

#endif