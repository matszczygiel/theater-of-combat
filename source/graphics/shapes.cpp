#include "shapes.h"

sf::Color site_color(HexType type) {
    switch (type) {
        case HexType::field:
            return sf::Color::Green;
        case HexType::forest:
            return sf::Color(100, 140, 20);
        default:
            static_assert(true);
    }
    return sf::Color::Magenta;
}

sf::Color site_color(RiverType type) {
    switch (type) {
        case RiverType::small:
            return sf::Color::Blue;
        case RiverType::stream:
            return sf::Color::Cyan;
        default:
            static_assert(true);
    }
    return sf::Color::Magenta;
}

const sf::ConvexShape& HexShape::shape() const { return _shape; }

const sf::ConvexShape& HexShape::highlighting_shape() const {
    return _highlighting_shape;
}

const sf::ConvexShape& HexShape::outline_shape() const {
    return _outline_shape;
}

HexShape::HexShape(std::shared_ptr<Layout> layout, const HexSite& site)
    : _layout{layout}, _shape{6}, _highlighting_shape{6}, _outline_shape{6} {
    update(site);

    _outline_shape.setOutlineColor(sf::Color::Black);
    _outline_shape.setFillColor(sf::Color::Transparent);

    _highlighting_shape.setOutlineThickness(0.f);
    _highlighting_shape.setFillColor(sf::Color(255, 0, 0, 120));

    _shape.setOutlineThickness(0.f);
}

void HexShape::update(const HexSite& site) {
    for (int i = 0; i < 6; ++i) {
        const auto offset = _layout->cornerr_offset(i);
        _shape.setPoint(i, offset);
        _highlighting_shape.setPoint(i, offset);
        _outline_shape.setPoint(i, offset);
    }

    const auto position = hex_to_world_point(site.coord(), *_layout);

    _shape.setPosition(position);
    _highlighting_shape.setPosition(position);
    _outline_shape.setPosition(position);

    const auto thickness = std::min(_layout->size.x, _layout->size.y) * 0.04f;
    _outline_shape.setOutlineThickness(thickness);

    _shape.setFillColor(site_color(site.type()));
}

RiverShape::RiverShape(std::shared_ptr<Layout> layout, const RiverSite& site)
    : _layout{layout}, _shape{4} {
    _shape.setOutlineThickness(0.0);
    update(site);
}

void RiverShape::update(const RiverSite& site) {
    const auto [coord1, coord2] = site.sides();

    const auto vec1 = hex_to_world_point(coord1, *_layout);
    const auto vec2 = hex_to_world_point(coord2, *_layout);

    const auto center         = (vec1 + vec2) / 2.f;
    const auto connecter      = (vec1 - vec2) / 2.f;
    const auto connecter_orth = sf::Vector2f{
        -connecter.y * _layout->size.x / _layout->size.y,
        connecter.x * _layout->size.y / _layout->size.x,
    };
    _shape.setPoint(0, 1.05f * connecter_orth / 2.f + 0.1f * connecter);
    _shape.setPoint(1, 1.05f * connecter_orth / 2.f - 0.1f * connecter);
    _shape.setPoint(2, -1.05f * connecter_orth / 2.f - 0.1f * connecter);
    _shape.setPoint(3, -1.05f * connecter_orth / 2.f + 0.1f * connecter);

    _shape.setPosition(center);

    _shape.setFillColor(site_color(site.type()));
}

const sf::ConvexShape& RiverShape::shape() const { return _shape; }
