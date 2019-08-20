#ifndef MAP_GFX_H
#define MAP_GFX_H

#include <map>
#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "map/hexagons.h"
#include "shapes.h"

struct MapGfx {
    void update(const Map& map);
    void draw_hexes(sf::RenderTarget& target) const;
    void draw_outlines(sf::RenderTarget& target) const;
    void draw_coords(sf::RenderTarget& target) const;
    void draw_rivers(sf::RenderTarget& target) const;

    std::shared_ptr<Layout> layout{new Layout{
        Orientation::Pointy, sf::Vector2f{0.f, 0.f}, sf::Vector2f{0.f, 0.f}}};

    std::vector<RiverShape> rivers{};
    std::map<HexCoordinate, HexShape> hexes{};

    sf::Font font{};
};

#endif