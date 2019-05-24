#include "mover.h"

#include <iostream>

#include <SFML/System/Vector2.hpp>

#include "log.h"
#include "map/passage_site.h"

#include "messaging/concrete_message.h"

void Mover::find_paths() {
    GAME_INFO("Initializing path finding algorithm.");

    create_table();

    const auto ocup   = _unit->get_ocupation();
    const auto mv_pts = _unit->get_mv_points();
    _paths.resize(mv_pts + 1);

    _paths[0].insert(ocup);
    ocup->set_highlighted(true);
    /* TODO
    implement finding the most optimal route
    eg Dijkstra algorithm
    */

    for (int i = 0; i <= mv_pts; ++i)
        for (auto& x : _paths[i])
            for (int j = 0; j < 6; ++j) {
                auto side = x->get_side(static_cast<Directions>(j));

                if (side != nullptr) {
                    if (!side->is_highlighted()) {
                        const auto st = side->get_site_type();
                        auto mp_cost  = i;

                        if (st == Site_type::hexagon) {
                            auto side1 = std::static_pointer_cast<Hex_site> (side);
                            mp_cost += _hex_table[side1->get_hex_type()];
                            if (mp_cost <= mv_pts) {
                                side1->set_highlighted(true);
                                _paths[mp_cost].insert(side1);
                            }

                        } else if (st == Site_type::passage) {
                            auto side1 = std::static_pointer_cast<Passage_site> (side);
                            mp_cost += _pass_table[side1->get_passage_type()];

                            auto side2 = side1->other_side(x);

                            if (!side2->is_highlighted()) {
                                mp_cost += _hex_table[side2->get_hex_type()];

                                if (mp_cost <= mv_pts) {
                                    side1->set_highlighted(true);
                                    side2->set_highlighted(true);
                                    _passages.insert(side1);
                                    _paths[mp_cost].insert(side2);
                                }
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
            auto hex = std::static_pointer_cast<Hex_site> (x);
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

    for (auto& x : _passages)
        x->set_highlighted(false);

    _unit = nullptr;
    _paths.clear();
    _passages.clear();
}