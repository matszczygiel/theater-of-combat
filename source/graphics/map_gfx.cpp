#include "map_gfx.h"

#include "core/log.h"

MapGfx::MapGfx(std::shared_ptr<Layout>& layout) {
    if (layout)
        _layout = layout;
}

void MapGfx::setup(const Map& map, std::string texture_path,
                   const std::map<HexType, sf::IntRect>& texture_positions) {
    _texture_positions = texture_positions;

    const auto loaded = _texture.loadFromFile(texture_path);
    engine_assert_throw(loaded, "Cannot load tiles texture from file: {}", texture_path);

    _texture.setSmooth(true);
    update(map);
}

void MapGfx::update(const Map& map) {
    clear();
    for (const auto& [id, site] : map.hexes()) {
        if (auto it = _texture_positions.find(site.type());
            it != _texture_positions.end()) {
            hexes.emplace_back(std::make_pair(
                site.coord(), HexShape{_layout, site, &_texture, it->second}));
        } else {
            hexes.emplace_back(std::make_pair(
                site.coord(), HexShape{_layout, site, nullptr, sf::IntRect()}));
        }
    }

    for (const auto& [id, site] : map.rivers()) {
        rivers.emplace_back(RiverShape{_layout, site});
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

void MapGfx::draw_coords(sf::RenderTarget& target, const sf::Font& font) const {
    for (const auto& [coord, shape] : hexes) {
        auto text =
            sf::Text{std::to_string(coord.q()) + "    " + std::to_string(coord.p()), font,
                     static_cast<unsigned int>(_layout->size.x * 0.3)};

        text.setFillColor(sf::Color::Magenta);
        text.setOutlineColor(sf::Color::Magenta);
        text.setStyle(sf::Text::Bold);
        text.setOrigin(text.getLocalBounds().width / 2.0, text.getLocalBounds().height);
        text.setPosition(shape.shape().getPosition());
        target.draw(text);
    }
}

void MapGfx::draw_rivers(sf::RenderTarget& target) const {
    for (const auto& shape : rivers) {
        target.draw(shape.shape());
    }
}

void MapGfx::clear() noexcept {
    rivers.clear();
    hexes.clear();
}
