#include "map/hexagons.h"

const Orientation Orientation::Pointy{
    {std::sqrt(3.f), std::sqrt(3.f) / 2.f, 0.f, 3.f / 2.f},
    {std::sqrt(3.f) / 3.f, -1.f / 3.f, 0.f, 2.f / 3.f},
    0.5};

const Orientation Orientation::Flat{
    {3.f / 2.f, 0.f, std::sqrt(3.f) / 2.f, std::sqrt(3.f)},
    {2.f / 3.f, 0.f, -1.f / 3.f, std::sqrt(3.f) / 3.f},
    0.f};

sf::Vector2f Layout::cornerr_offset(int cornerr) const noexcept {
    const float angle = 2.0 * M_PI * (orientation.start_angle + cornerr) / 6.0;
    return sf::Vector2f{size.x * std::cos(angle), size.y * std::sin(angle)};
}

sf::Vector2f hex_to_world_point(const HexCoordinate& hex, const Layout& layout) noexcept {
    const auto& m = layout.orientation.m;
    const auto x  = (m[0] * hex.q() + m[1] * hex.p()) * layout.size.x;
    const auto y  = (m[2] * hex.q() + m[3] * hex.p()) * layout.size.y;
    return sf::Vector2f{x, y} + layout.origin;
}

HexCoordinate world_point_to_hex(sf::Vector2f point, const Layout& layout) noexcept {
    const sf::Vector2f pt{(point.x - layout.origin.x) / layout.size.x,
                          (point.y - layout.origin.y) / layout.size.y};
    const auto& m = layout.orientation.minv;
    const float q = m[0] * pt.x + m[1] * pt.y;
    const float p = m[2] * pt.x + m[3] * pt.y;
    auto hex      = HexCoordinateFractional(q, p);
    return round(hex);
}