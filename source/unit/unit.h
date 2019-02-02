#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "map/hex_site.h"

class Unit {
   public:
    explicit Unit() : _moving_pts(5), _current_moving_pts(_moving_pts) {}

    virtual void draw(sf::RenderTarget& window);
    virtual void init_token(const float& size);

    void place_on_hex(Hex_site* hex);
    void reset_mv_points();

    inline auto get_ocupation() const { return _ocupation; }
    inline auto get_mv_points() const { return _current_moving_pts; }

    inline bool token_contain(const sf::Vector2f& vec) {
        return _token.getGlobalBounds().contains(vec);
    }

    inline void reduce_mv_points(int points) {
        _current_moving_pts -= points;
    }

   private:
    Hex_site* _ocupation;
    
    sf::RectangleShape _token;
    const int _moving_pts;
    int _current_moving_pts;
};