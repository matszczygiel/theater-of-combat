#include "concrete_passage.h"

#include <cmath>

#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

#include "log.h"

River::River(const int &number)
    : Passage_site(number), _shape(0.0, 0.2) {}

void River::set_shape(const sf::Vector2f &pos1, const sf::Vector2f &pos2,
                      const float &radius) {
    const auto vec = pos1 - pos2;
    const auto distance =
        std::sqrt(vec.x * vec.x + vec.y * vec.y);

    if (distance > 2 * radius) {
        GAME_ERROR("Setting the shape of river failled. distance = {0}", distance);
        throw std::runtime_error("Setting the shape of river failled.");
    }

    const auto angle = 180.f / M_PI * std::atan(vec.y / vec.x);
    _shape.set_length(radius);
    _shape.setPosition((pos1 + pos2) / 2.f);
    _shape.setRotation(angle);
    _shape.setFillColor(sf::Color::Blue);
}

void River::draw(sf::RenderTarget &target) const {
    target.draw(_shape);
}

CEREAL_REGISTER_TYPE(River);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Passage_site, River);


