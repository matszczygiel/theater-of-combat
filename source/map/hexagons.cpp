#include "hexagons.h"

#include <cmath>

HexCoordinate round(const HexCoordinateFractional &hex) {
    int rx = std::lround(hex.x());
    int ry = std::lround(hex.y());
    int rz = std::lround(hex.z());

    const auto x_diff = std::abs(rx - hex.x());
    const auto y_diff = std::abs(ry - hex.y());
    const auto z_diff = std::abs(rz - hex.z());

    if (x_diff > y_diff && x_diff > z_diff) {
        rx = -ry - rz;
    } else if (y_diff > z_diff) {
        ry = -rx - rz;
    } else {
        rz = -rx - ry;
    }
    return HexCoordinate(rx, ry, rz);
}

const Orientation Orientation::Pointy{
    {std::sqrt(3.f), std::sqrt(3.f) / 2.f, 0.f, 3.f / 2.f},
    {std::sqrt(3.f) / 3.f, -1.f / 3.f, 0.f, 2.f / 3.f},
    0.5};

const Orientation Orientation::Flat{
    {3.f / 2.f, 0.f, std::sqrt(3.f) / 2.f, std::sqrt(3.f)},
    {2.f / 3.f, 0.f, -1.f / 3.f, std::sqrt(3.f) / 3.f},
    0.f};

sf::Vector2f Layout::cornerr_offset(int cornerr) const {
    const float angle = 2.0 * M_PI * (orientation.start_angle + cornerr) / 6.0;
    return sf::Vector2f{size.x * std::cos(angle), size.y * std::sin(angle)};
}

sf::Vector2f hex_to_world_point(const HexCoordinate &hex,
                                const Layout &layout) {
    const auto &m = layout.orientation.m;
    const auto x  = m[0] * hex.q() + m[1] * hex.p() * layout.size.x;
    const auto y  = m[2] * hex.q() + m[3] * hex.p() * layout.size.y;
    return sf::Vector2f{x, y} + layout.origin;
}

HexCoordinate hex_to_world_point(sf::Vector2f point, const Layout &layout) {
    const sf::Vector2f pt{(point.x - layout.origin.x) / layout.size.x,
                          (point.y - layout.origin.y) / layout.size.y};
    const auto &m = layout.orientation.minv;
    const float q = m[0] * pt.x + m[1] * pt.y;
    const float p = m[2] * pt.x + m[3] * pt.y;
    auto hex      = HexCoordinateFractional(q, p);
    return round(hex);
}
