#include <cmath>

#include "hex_site.h"
#include "log.h"

std::string type_to_string(const Hex_type& type) {
    switch (type) {
        case Hex_type::field:
            return "field";
        case Hex_type::forest:
            return "forest";
        case Hex_type::hills:
            return "hills";
        case Hex_type::mountains:
            return "mountains";
        case Hex_type::swamp:
            return "swamp";
        default:
            GAME_WARN("Invalid hex type to string conversion.");
            return "";
    }
}

Hex_type string_to_hex_type(const std::string& str) {
    if (str == "field")
        return Hex_type::field;
    else if (str == "forest")
        return Hex_type::forest;
    else if (str == "hills")
        return Hex_type::hills;
    else if (str == "mountains")
        return Hex_type::mountains;
    else if (str == "swamp")
        return Hex_type::swamp;
    else {
        GAME_ERROR("Invalid string to hex type conversion.");
        throw std::runtime_error("Invalid string to hex type conversion.");
    }
}

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

void Hex_site::write(pugi::xml_node& node) {
    auto hex_node = node.append_child("hex");

    hex_node.append_attribute("number").set_value(_number);
    hex_node.append_attribute("hex_type").set_value(type_to_string(get_hex_type()).c_str());

    auto sides_node = hex_node.append_child("sides_array");

    for (int s = 0; s < 6; ++s) {
        const auto& side = get_side(static_cast<Directions>(s));

        if (side != nullptr) {
            auto single_side_node = sides_node.append_child("side");
            single_side_node.append_attribute("direction").set_value(direction_to_string(static_cast<Directions>(s)).c_str());
            side->write(single_side_node);
        }
    }
}