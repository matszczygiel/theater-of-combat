#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "map/hex_site.h"
#include "unit_type.h"

class Mover;

class Unit {
   public:
    explicit Unit(const int& moving_pts)
        : _ocupation(), _token(), _moving_pts(moving_pts), _current_moving_pts(moving_pts) {}

    Unit()                   = delete;
    virtual ~Unit() noexcept = default;

    virtual void draw(sf::RenderTarget& window);
    inline virtual Unit_type get_type() const = 0;
    inline virtual void set_color()           = 0;
    inline virtual Mover* get_mover()                  = 0;

    void place_on_hex(Hex_site* hex);
    void reset_mv_points() noexcept;
    void init_token(const float& size);
    void reduce_mv_points(const int& points);

    inline auto get_ocupation() const { return _ocupation; }
    inline auto get_mv_points() const { return _current_moving_pts; }

    inline bool token_contain(const sf::Vector2f& vec) {
        return _token.getGlobalBounds().contains(vec);
    }

   protected:
    Hex_site* _ocupation;

    sf::RectangleShape _token;
    const int _moving_pts;
    int _current_moving_pts;
};