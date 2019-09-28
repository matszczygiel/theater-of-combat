#ifndef UNIT_GFX_H
#define UNIT_GFX_H

#include <memory>
#include <vector>
#include <map>

#include <SFML/Graphics.hpp>

#include "map_gfx.h"
#include "token.h"
#include "unit/unit_manager.h"

struct UnitGfx {
    UnitGfx(std::shared_ptr<Layout>& layout);
    void setup(UnitManager& manager, std::string texture_path,
               const std::map<Unit::IdType, sf::IntRect>& texture_positions);
    void update(UnitManager& manager);
    void draw_tokens(sf::RenderTarget& target) const;
    void draw_ids(sf::RenderTarget& target, const sf::Font& font) const;

    void clear();

    std::vector<std::pair<Unit::IdType, Token>> tokens{};

   private:
    std::map<Unit::IdType, Token> _all_tokens{};
    std::shared_ptr<Layout> _layout{};
    sf::Texture _texture{};
};

#endif