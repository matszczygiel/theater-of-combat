#include <cmath>

#include "hex_site.h"

Hex_site::Hex_site(const int& number)
    : Map_site(number), _shape(), _sides{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr} {};

void Hex_site::set_shape(const float& x, const float& y, const float& radius) {
    _shape = Hex_shape(radius);
    set_color();
    _shape.setPosition(x, y);
    _shape.setOutlineThickness(-0.05 * radius);
    _shape.setOutlineColor(sf::Color::Black);
}

void Hex_site::draw(sf::RenderTarget& target) const {
    target.draw(_shape);
}

bool Hex_site::contains(const sf::Vector2f& vec) const noexcept {
    const auto rel_pos = vec - _shape.getPosition();
    return std::sqrt(std::pow(rel_pos.x, 2) + std::pow(rel_pos.y, 2)) < get_small_radius();
}

void Hex_site::set_highlighted(bool highlighted) noexcept {
    _highlighted = highlighted;
    if (_highlighted)
        _shape.setOutlineColor(sf::Color::Red);
    else
        _shape.setOutlineColor(sf::Color::Black);
}
