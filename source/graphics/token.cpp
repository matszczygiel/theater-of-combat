#include "token.h"

Token::Token(std::shared_ptr<Layout> layout, const HexCoordinate& site)
    : _layout{layout}, _shape{4}, _highlighting_shape{4} {
    _highlighting_shape.setOutlineThickness(0.f);
    _highlighting_shape.setFillColor(sf::Color(255, 0, 0, 120));
    _shape.setFillColor(sf::Color::Red);
    _shape.setOutlineColor(sf::Color::Black);

    update(site);
}

void Token::update(const HexCoordinate& site) {
    const auto size = _layout->size;

    _shape.setPoint(0, {size.x / 2, -size.y / 2});
    _shape.setPoint(1, {size.x / 2, size.y / 2});
    _shape.setPoint(2, {-size.x / 2, size.y / 2});
    _shape.setPoint(3, {-size.x / 2, -size.y / 2});

    _highlighting_shape.setPoint(0, {size.x, 0});
    _highlighting_shape.setPoint(1, {size.x, size.y});
    _highlighting_shape.setPoint(2, {0, size.y});
    _highlighting_shape.setPoint(3, {0, 0});

    const auto position = hex_to_world_point(site, *_layout);
    _shape.setPosition(position);
    _highlighting_shape.setPosition(position);

    const auto thickness = std::min(_layout->size.x, _layout->size.y) * 0.04f;
    _shape.setOutlineThickness(thickness);
}

const sf::ConvexShape& Token::shape() const { return _shape; }

const sf::ConvexShape& Token::highlighting_shape() const {
    return _highlighting_shape;
}