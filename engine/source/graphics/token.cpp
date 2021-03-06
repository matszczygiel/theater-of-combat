#include "graphics/token.h"

Token::Token(std::shared_ptr<Layout> layout, const HexCoordinate& site, float rotation,
             const sf::Texture* texture, const sf::IntRect& texture_rect) noexcept
    : _layout{layout}, _shape{4}, _highlighting_shape{4} {
    _highlighting_shape.setOutlineThickness(0.f);
    _highlighting_shape.setFillColor(sf::Color(255, 0, 0, 120));
    if (texture) {
        _shape.setTexture(texture);
        _shape.setTextureRect(texture_rect);
    } else {
        _shape.setFillColor(sf::Color::Magenta);
    }
    update(site, rotation);
}

void Token::update(const HexCoordinate& site, float rotation) noexcept {
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

    constexpr float pi         = 3.14159265358979323846f;
    constexpr float rad_to_deg = 360.f / 2.f / pi;
    const auto angle           = 90.f - rotation * rad_to_deg;
    _shape.setRotation(angle);
    _highlighting_shape.setRotation(angle);
}

const sf::ConvexShape& Token::shape() const noexcept { return _shape; }

const sf::ConvexShape& Token::highlighting_shape() const noexcept {
    return _highlighting_shape;
}