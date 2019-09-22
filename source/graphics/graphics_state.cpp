#include "graphics_state.h"

GfxState::GfxState(const GameState& state)
    : _map{state.scenario.map}, _units{state.scenario.units} {
    update();
}

void GfxState::update() {
    map.update(*_map);
    units.update(*_units);
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
    if(debug_units)
        units.draw_ids(target, font);
}
