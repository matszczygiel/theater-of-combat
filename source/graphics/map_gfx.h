#ifndef MAP_GFX_H
#define MAP_GFX_H

#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "map/hexagons.h"
#include "shapes.h"

struct MapGfx {
    MapGfx(std::shared_ptr<Layout>& layout);

    void update(const Map& map);
    void draw_hexes(sf::RenderTarget& target) const;
    void draw_outlines(sf::RenderTarget& target) const;
    void draw_coords(sf::RenderTarget& target) const;
    void draw_rivers(sf::RenderTarget& target) const;

    std::vector<RiverShape> rivers{};
    std::vector<std::pair<HexCoordinate, HexShape>> hexes{};

    sf::Font font{};

   private:
    std::shared_ptr<Layout> _layout{new Layout{
        Orientation::Pointy, sf::Vector2f{0.f, 0.f}, sf::Vector2f{0.f, 0.f}}};
};

#endif