#include "map_gfx.h"

void MapGfx::update(const Map& map) {
    hexes.clear();
    for (const auto& [id, site] : map.hexes()) {
        hexes.emplace_back(
            std::make_pair(site.coord(), HexShape{layout, site}));
    }

    rivers.clear();
    for (const auto& [id, site] : map.rivers()) {
        rivers.emplace_back(RiverShape{layout, site});
    }
}

void MapGfx::draw_hexes(sf::RenderTarget& target) const {
    for (const auto& [coord, shape] : hexes) {
        target.draw(shape.shape());
    }
}

void MapGfx::draw_outlines(sf::RenderTarget& target) const {
    for (const auto& [coord, shape] : hexes) {
        target.draw(shape.outline_shape());
    }
}

void MapGfx::draw_coords(sf::RenderTarget& target) const {
    for (const auto& [coord, shape] : hexes) {
        auto text = sf::Text{
            std::to_string(coord.q()) + "  " + std::to_string(coord.p()), font,
            static_cast<unsigned int>(layout->size.x * 0.5)};

        text.setFillColor(sf::Color::Magenta);
        text.setOutlineColor(sf::Color::Magenta);
        text.setStyle(sf::Text::Bold);
        text.setOrigin(text.getLocalBounds().width / 2.0,
                       text.getLocalBounds().height);
        text.setPosition(shape.shape().getPosition());
        target.draw(text);
    }
}

void MapGfx::draw_rivers(sf::RenderTarget& target) const {
    for (const auto& shape : rivers) {
        target.draw(shape.shape());
    }
}
