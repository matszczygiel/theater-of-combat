#pragma once

#include <SFML/Graphics/RenderTarget.hpp>

#include "directions.h"
#include "site_type.h"

class Map_site {
   public:
    static inline Directions opposite_direction(const Directions& dir) {
        return Directions((static_cast<int>(dir) + 3) % 6);
    }

    virtual inline void set_side(const Directions& side, Map_site* site) {
        set_side_this(side, site);
        site->set_side_this(opposite_direction(side), this);
    }

    virtual void draw(sf::RenderTarget& target) const        = 0;
    virtual void set_highlighted(bool highlighted)           = 0;
    virtual Map_site* get_side(const Directions& side) const = 0;
    virtual Site_type get_type() const                       = 0;

    inline bool is_highlighted() const { return _highlighted; }
    inline const auto& get_number() const { return _number; }

   protected:
    virtual void set_side_this(const Directions& side, Map_site* site) = 0;

    bool _highlighted;
    int _number;
};