#include "graphics_state.h"

GfxState::GfxState(const GameState& state) : _scenario{state.scenario} { update(); }

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
