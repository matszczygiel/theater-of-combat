#include "graphics/graphics_state.h"

GfxState::GfxState(const std::shared_ptr<Scenario>& sc) : _scenario{sc} { update(); }

void GfxState::update() {
    if (!_scenario) {
        map.clear();
        units.clear();
        return;
    }

    map.update(_scenario->map);
    units.update(_scenario->units);
}

void GfxState::draw(sf::RenderTarget& target) const {
    map.draw_hexes(target);
    map.draw_rivers(target);
    map.draw_outlines(target);

    for (const auto& shape : highlighted_hexes) {
        target.draw(shape);
    }

    if (debug_coords)
        map.draw_coords(target, font);

    units.draw_tokens(target);
    if (debug_units)
        units.draw_ids(target, font);
}

void GfxState::highlight_hex(HexCoordinate coord) {
    if (!map.hexes.empty()) {
        if (auto it =
                std::find_if(map.hexes.cbegin(), map.hexes.cend(),
                             [&](const auto& hex) { return hex.first == coord; });
            it != map.hexes.cend()) {
            highlighted_hexes.push_back(it->second.highlighting_shape());
        }
    }
}