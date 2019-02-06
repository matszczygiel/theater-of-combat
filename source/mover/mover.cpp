#include "mover.h"

#include <iostream>

#include <SFML/System/Vector2.hpp>

#include "log.h"

void Mover::find_paths() {
    GAME_INFO("Initializing path finding algorithm.");

    create_table();

    const auto ocup   = _unit->get_ocupation();
    const auto mv_pts = _unit->get_mv_points();
    _paths.resize(mv_pts);

    _paths[0].insert(ocup);
    ocup->set_highlighted(true);
    /* TODO
    implement algorith which can distinguish between hex_sites and pure map_sites
    */

    for (int i = 0; i < mv_pts; ++i) {
        for (auto& x : _paths[i]) {
            for (int j = 0; j < 6; ++j) {
                auto side = x->get_side(static_cast<Directions>(j));
                if (side != nullptr)
                    if (!side->is_highlighted()) {
                        const auto mp_cost = i + _table[side->get_type()];
                        if (mp_cost < mv_pts) {
                            side->set_highlighted(true);
                            _paths[mp_cost].insert(side);
                        }
                    }
            }
        }
    }
}

void Mover::move(const sf::Vector2f& mouse_pos) {
    GAME_INFO("Moving unit.");
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