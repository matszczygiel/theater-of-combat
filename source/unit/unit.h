#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "map/hex_site.h"
#include "unit_type.h"

class Unit {
   public:
    explicit Unit(const int& moving_pts)
        : _moving_pts(moving_pts), _current_moving_pts(_moving_pts) {}

    virtual ~Unit() = default;

    virtual void draw(sf::RenderTarget& window);
    virtual Unit_type get_type() const = 0;
    virtual void set_color()           = 0;

    void place_on_hex(Hex_site* hex);
    void reset_mv_points();
    void init_token(const float& size);

    inline auto get_ocupation() const { return _ocupation; }
    inline auto get_mv_points() const { return _current_moving_pts; }

    inline bool token_contain(const sf::Vector2f& vec) {
        return _token.getGlobalBounds().contains(vec);
    }

    inline void reduce_mv_points(int points) {
        _current_moving_pts -= points;
    }

   protected:
    Hex_site* _ocupation;

    sf::RectangleShape _token;
    const int _moving_pts;
    int _current_moving_pts;
};