#pragma once

#include <SFML/Graphics/RenderTarget.hpp>

#include "directions.h"

enum class Site_type {
    hexagon,
    passage
};

class Map_site {
   public:
    explicit Map_site(const int& number = 0)
        : _highlighted(false), _number(number) {}

    virtual ~Map_site() = default;

    virtual void set_highlighted(bool highlighted) noexcept = 0;
    virtual void draw(sf::RenderTarget& target) const       = 0;
    virtual Site_type get_site_type() const                 = 0;

    bool is_highlighted() const;
    const auto& get_number() const;

    static Directions opposite_direction(const Directions& dir);

   protected:
    bool _highlighted;
    int _number;
};

inline bool Map_site::is_highlighted() const { return _highlighted; }

inline const auto& Map_site::get_number() const { return _number; }

inline Directions Map_site::opposite_direction(const Directions& dir) {
    return Directions((static_cast<int>(dir) + 3) % 6);
}
