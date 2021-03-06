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
    constexpr float pi = 3.14159265358979323846f;
    const float angle  = 2.0f * pi * (orientation.start_angle + cornerr) / 6.0f;
    return sf::Vector2f{size.x * std::cos(angle), size.y * std::sin(angle)};
}

float Layout::direction_angle(int direction) const noexcept {
    constexpr float pi = 3.14159265358979323846f;
    const auto edge    = direction % 6;
    return 2.0f * pi * (edge + orientation.start_angle - 0.5f) / 6.0f;
}

sf::Vector2f hex_to_world_point(const HexCoordinate& hex, const Layout& layout) noexcept {
    const auto& m = layout.orientation.m;
    const auto x  = (m[0] * hex.q() + m[1] * hex.r()) * layout.size.x;
    const auto y  = (m[2] * hex.q() + m[3] * hex.r()) * layout.size.y;
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

HexCoordinate round(const HexCoordinateFractional& hex) noexcept {
    int rx = std::lround(hex.x());
    int ry = std::lround(hex.y());
    int rz = std::lround(hex.z());

    const auto x_diff = std::abs(rx - hex.x());
    const auto y_diff = std::abs(ry - hex.y());
    const auto z_diff = std::abs(rz - hex.z());

    if (x_diff > y_diff && x_diff > z_diff) {
        rx = -ry - rz;
    } else if (y_diff <= z_diff) {
        rz = -rx - ry;
    }
    return HexCoordinate(rx, rz);
}
