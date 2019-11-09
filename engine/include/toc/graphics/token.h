#ifndef TOKEN_H
#define TOKEN_H

#include <memory>
#include <optional>

#include <SFML/Graphics.hpp>

#include "toc/map/hexagons.h"

class Token {
   public:
    Token(std::shared_ptr<Layout> layout, const HexCoordinate& site, float rotation,
          const sf::Texture* texture, const sf::IntRect& texture_rect) noexcept;

    void update(const HexCoordinate& site, float rotation) noexcept;

    const sf::ConvexShape& shape() const noexcept;
    const sf::ConvexShape& highlighting_shape() const noexcept;

   private:
    std::shared_ptr<Layout> _layout{};
    sf::ConvexShape _shape{};
    sf::ConvexShape _highlighting_shape{};
};

#endif