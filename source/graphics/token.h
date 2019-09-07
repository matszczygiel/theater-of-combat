#ifndef TOKEN_H
#define TOKEN_H

#include <memory>
#include <optional>

#include <SFML/Graphics.hpp>

#include "map/hexagons.h"

class Token {
   public:
    Token(std::shared_ptr<Layout> layout);

    void update(const HexCoordinate& site);

    const sf::ConvexShape& shape() const;
    const sf::ConvexShape& highlighting_shape() const;

   private:
    std::shared_ptr<Layout> _layout;
    sf::ConvexShape _shape;
    sf::ConvexShape _highlighting_shape;
};

#endif