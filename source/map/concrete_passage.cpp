#include "concrete_passage.h"

#include <cmath>

#include "log.h"

void River::set_shape() {
    const auto sid1 = std::get<0>(_sides);
    const auto sid2 = std::get<1>(_sides);

    if (sid1 == nullptr || sid2 == nullptr) {
        GAME_ERROR("Setting the shape of passage failled. Sides not initialized");
        throw std::runtime_error("Setting the shape of passage failled.");
    }
    const auto& rad1 = sid1->get_radius();
    const auto& rad2 = sid2->get_radius();

    if (rad1 != rad2) {
        GAME_ERROR("Setting the shape of passage failled. rad1 = {0}, rad2 = {1}", rad1, rad2);
        throw std::runtime_error("Setting the shape of passage failled.");
    }

    const auto& pos1 = sid1->get_position();
    const auto& pos2 = sid2->get_position();

    const auto vec = pos1 - pos2;
    const auto distance =
        std::sqrt(vec.x * vec.x + vec.y * vec.y);

    if (distance > 2 * rad1) {
        GAME_ERROR("Setting the shape of passage failled. distance = {0}", distance);
        throw std::runtime_error("Setting the shape of passage failled.");
    }

    const auto angle = std::atan(vec.y / vec.x);
    const auto smr   = sid1->get_small_radius();
    _shape.setPosition((pos1 + pos2) / 2.f - sf::Vector2f(smr / 2.f, rad1 * 0.1f));
    _shape.setSize(sf::Vector2f(rad1 * 0.2f, smr));
    _shape.rotate(angle + M_PI / 2.f);
    _shape.setFillColor(sf::Color::Blue);
}