#ifndef UNIT_GFX_H
#define UNIT_GFX_H

#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>

#include "token.h"
#include "unit/unit_manager.h"
#include "map_gfx.h"

struct UnitGfx {
    UnitGfx(std::shared_ptr<Layout>& layout);
    void update(UnitManager& manager);
    void draw_tokens(sf::RenderTarget& target) const;
    void draw_ids(sf::RenderTarget& target, const sf::Font& font) const;

    std::vector<std::pair<Unit::IdType, Token>> tokens{};

   private:
    std::shared_ptr<Layout> _layout{};
};

#endif