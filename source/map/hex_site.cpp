#include "hex_site.h"

#include <cmath>

#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

Hex_site::Hex_site(const int& number)
    : Map_site(number), _shape() {
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
    Map_site::set_highlighted(highlighted);
    if (highlighted)
        _shape.setOutlineColor(sf::Color::Red);
    else
        _shape.setOutlineColor(sf::Color::Black);
}

const float& Hex_site::get_radius() const {
    return _shape.get_radius();
}

float Hex_site::get_small_radius() const {
    return _shape.get_small_radius();
}

const sf::Vector2f& Hex_site::get_position() const {
    return _shape.getPosition();
}

CEREAL_REGISTER_TYPE(Hex_site)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Map_site, Hex_site);