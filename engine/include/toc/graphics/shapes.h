#ifndef SHAPES_H
#define SHAPES_H

#include <memory>

#include <SFML/Graphics.hpp>

#include "toc/map/hexagons.h"
#include "toc/map/map.h"
#include "toc/map/types.h"

class HexShape {
   public:
    HexShape(std::shared_ptr<Layout> layout, const HexSite& site,
             const sf::Texture* texture, const sf::IntRect& texture_rect) noexcept;

    void update(const HexSite& site) noexcept;

    const sf::ConvexShape& shape() const noexcept;
    const sf::ConvexShape& highlighting_shape() const noexcept;
    const sf::ConvexShape& outline_shape() const noexcept;

   private:
    std::shared_ptr<Layout> _layout;
    sf::ConvexShape _shape;
    sf::ConvexShape _highlighting_shape;
    sf::ConvexShape _outline_shape;
};

class RiverShape {
   public:
    RiverShape(std::shared_ptr<Layout> layout, const RiverSite& site) noexcept;

    void update(const RiverSite& site) noexcept;

    const sf::ConvexShape& shape() const noexcept;

   private:
    std::shared_ptr<Layout> _layout;
    sf::ConvexShape _shape;
};

#endif