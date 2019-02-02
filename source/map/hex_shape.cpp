#include "hex_shape.h"

#include <cmath>

Hex_shape::Hex_shape(const float& radius) : _radius(radius) {
    update();
}

void Hex_shape::set_radius(const float& radius) {
    _radius = radius;
    update();
}

const float& Hex_shape::get_radius() const {
    return _radius;
}

const float Hex_shape::get_small_radius() const {
    return _radius * std::cos(M_PI / 6.);
}

sf::Vector2f Hex_shape::getPoint(std::size_t index) const {
    float angle = index * 2 * M_PI / 6. + M_PI / 6.;
    float x     = std::cos(angle) * _radius;
    float y     = std::sin(angle) * _radius;

    return sf::Vector2f(x, y);
}

float Hex_shape::get_small_radius(const float& radius) {
    return radius * std::cos(M_PI / 6.);
}
