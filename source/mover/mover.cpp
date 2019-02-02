#include "mover.h"

#include <iostream>

#include <SFML/System/Vector2.hpp>

#include "log.h"

void Mover::find_paths() {
    GAME_INFO("Initializing path finding algorithm.");

    const auto ocup   = _unit->get_ocupation();
    const auto mv_pts = _unit->get_mv_points();
    _paths.resize(mv_pts);

    _paths[0].insert(ocup);
    ocup->set_highlighted(true);
    /* TODO
    implement algorith which can distinguish between hex_sites and pure map_sites
    */

    for (int i = 1; i < mv_pts; ++i) {
        for (auto& x : _paths[i - 1]) {
            for (int j = 0; j < 6; ++j) {
                auto side = x->get_side(static_cast<Directions>(j));
                if (side != nullptr)
                    if (!side->is_highlighted()) {
                        side->set_highlighted(true);
                        _paths[i].insert(side);
                    }
            }
        }
    }
}

void Mover::move(const sf::Vector2f& mouse_pos) {
    for (int used_mp = 0; used_mp < _paths.size(); ++used_mp) {
        for (auto& x : _paths[used_mp]) {
            auto hex = static_cast<Hex_site*>(x);
            if (hex->contains(mouse_pos)) {
                _unit->place_on_hex(hex);
                _unit->reduce_mv_points(used_mp);

                clear();
                return;
            }
        }
    }
    clear();
    return;
}

void Mover::clear() {
    for (auto& vec : _paths)
        for (auto& y : vec)
            y->set_highlighted(false);
    _unit = nullptr;
    _paths.clear();
}