#ifndef UNIT_GFX_H
#define UNIT_GFX_H

#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>

#include "map/hexagons.h"
#include "token.h"
#include "unit/unit_manager.h"
#include "map_gfx.h"

struct UnitGfx {
    UnitGfx(MapGfx& map);
    void update(UnitManager& manager);
    void draw_tokens(sf::RenderTarget& target) const;

    std::vector<std::pair<Unit::IdType, Token>> tokens{};

   private:
    std::shared_ptr<Layout> _layout{};
};

#endif