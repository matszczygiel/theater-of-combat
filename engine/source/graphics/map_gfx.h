#ifndef MAP_GFX_H
#define MAP_GFX_H

#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "map/hexagons.h"
#include "shapes.h"

struct MapGfx {
    MapGfx(std::shared_ptr<Layout>& layout) ;

    void update(const Map& map);
    void setup(const Map& map, std::string texture_path,
               const std::map<HexType, sf::IntRect>& texture_positions);

    void draw_hexes(sf::RenderTarget& target) const;
    void draw_outlines(sf::RenderTarget& target) const;
    void draw_coords(sf::RenderTarget& target, const sf::Font& font) const;
    void draw_rivers(sf::RenderTarget& target) const;

    void clear() noexcept;

    std::vector<RiverShape> rivers{};
    std::vector<std::pair<HexCoordinate, HexShape>> hexes{};

   private:
    std::shared_ptr<Layout> _layout{
        new Layout{Orientation::Pointy, sf::Vector2f{0.f, 0.f}, sf::Vector2f{0.f, 0.f}}};

    sf::Texture _texture{};
    std::map<HexType, sf::IntRect> _texture_positions{};
};

#endif