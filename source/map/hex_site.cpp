#include <cmath>

#include "hex_site.h"

void Hex_site::set_shape(float x, float y, float radius) {
    _shape = Hex_shape(radius);
    _shape.setFillColor(sf::Color::White);
    _shape.setPosition(x, y);
    _shape.setOutlineThickness(-0.05 * radius);
    _shape.setOutlineColor(sf::Color::Black);
}

void Hex_site::draw(sf::RenderTarget& target) const {
    target.draw(_shape);
}

bool Hex_site::contains(const sf::Vector2f& vec) const {
    const auto rel_pos = vec - _shape.getPosition();
    return std::sqrt(std::pow(rel_pos.x, 2) + std::pow(rel_pos.y, 2)) < get_small_radius();
}

void Hex_site::set_side_this(const Directions& side, Map_site* site) {
    _sides[static_cast<int>(side)] = site;
}

void Hex_site::set_highlighted(bool highlighted) {
    _highlighted = highlighted;
    if (_highlighted)
        _shape.setOutlineColor(sf::Color::Red);
    else
        _shape.setOutlineColor(sf::Color::Black);
}
