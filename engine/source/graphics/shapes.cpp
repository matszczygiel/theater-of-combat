#include "graphics/shapes.h"

#include "core/log.h"

static sf::Color hex_color(HexSite::HexType type) noexcept {
    switch (type) {
        case 0:
            return sf::Color::Green;
        case 1:
            return sf::Color(100, 140, 20);
        default:
            engine_assert(false, "Unknown HexType {}.", type);
    }
    return sf::Color::Magenta;
}

static sf::Color border_color(BorderSite::BorderType type) noexcept {
    switch (type) {
        case 0:
            return sf::Color::Blue;
        case 1:
            return sf::Color::Cyan;
        default:
            engine_assert(false, "Unknown BorderType {}.", type);
    }
    return sf::Color::Magenta;
}

const sf::ConvexShape& HexShape::shape() const noexcept { return _shape; }

const sf::ConvexShape& HexShape::highlighting_shape() const noexcept {
    return _highlighting_shape;
}

const sf::ConvexShape& HexShape::outline_shape() const noexcept { return _outline_shape; }

HexShape::HexShape(std::shared_ptr<Layout> layout, const HexSite& site,
                   const sf::Texture* texture, const sf::IntRect& texture_rect) noexcept
    : _layout{layout}, _shape{6}, _highlighting_shape{6}, _outline_shape{6} {
    update(site);

    _outline_shape.setOutlineColor(sf::Color::Black);
    _outline_shape.setFillColor(sf::Color::Transparent);

    _highlighting_shape.setOutlineThickness(0.f);
    _highlighting_shape.setFillColor(sf::Color(255, 0, 0, 120));

    _shape.setOutlineThickness(0.f);
    if (texture) {
        _shape.setTexture(texture);
        _shape.setTextureRect(texture_rect);
    } else {
        _shape.setFillColor(hex_color(site.type()));
    }
}

void HexShape::update(const HexSite& site) noexcept {
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
}

BorderShape::BorderShape(std::shared_ptr<Layout> layout, const BorderSite& site) noexcept
    : _layout{layout}, _shape{4} {
    _shape.setOutlineThickness(0.0);
    update(site);
}

void BorderShape::update(const BorderSite& site) noexcept {
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

    _shape.setFillColor(border_color(site.type()));
}

const sf::ConvexShape& BorderShape::shape() const noexcept { return _shape; }
