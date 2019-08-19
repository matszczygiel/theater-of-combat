#ifndef SHAPES_H
#define SHAPES_H

#include <memory>

#include <SFML/Graphics.hpp>

#include "map/hexagons.h"
#include "map/map.h"
#include "map/types.h"

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

class HexShape {
   public:
    HexShape(std::shared_ptr<Layout> layout, const HexSite& site);

    void update(const HexSite& site);

    const sf::ConvexShape& shape() const;
    const sf::ConvexShape& highlighting_shape() const;
    const sf::ConvexShape& outline_shape() const;

   private:
    std::shared_ptr<Layout> _layout;
    sf::ConvexShape _shape;
    sf::ConvexShape _highlighting_shape;
    sf::ConvexShape _outline_shape;
};

class RiverShape {
   public:
    RiverShape(std::shared_ptr<Layout> layout, const RiverSite& site);

    void update(const RiverSite& site);

    const sf::ConvexShape& shape() const;

   private:
    std::shared_ptr<Layout> _layout;
    sf::ConvexShape _shape;
};

#endif