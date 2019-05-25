#include "hex_site.h"

#include <cmath>

Hex_site::Hex_site(const int& number)
    : Map_site(number), _shape() {
}

Hex_site::Hex_site(pugi::xml_node& node)
    : Map_site(node), _shape() {
}

void Hex_site::set_shape(const float& x, const float& y, const float& radius) {
    _shape = Hex_shape(radius);
    _shape.setFillColor(color());
    _shape.setPosition(x, y);
    _shape.setOutlineThickness(-0.05 * radius);
    _shape.setOutlineColor(sf::Color::Black);
}

void Hex_site::draw(sf::RenderTarget& target) const {
    target.draw(_shape);
}

void Hex_site::draw_number(sf::RenderTarget& target, const sf::Font& font) const {
    sf::Text num_text(std::to_string(get_number()), font, 10);
    num_text.setPosition(_shape.getPosition());
    num_text.setFillColor(sf::Color::Black);
    target.draw(num_text);
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

