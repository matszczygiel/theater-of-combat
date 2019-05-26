#pragma once

#include "shapes/hex_border_shape.h"
#include "map_site.h"

#include <SFML/System/Vector2.hpp>

class River : public Map_site {
   public:
    explicit River(const int &number = 0) : Map_site(number) {}
    explicit River(pugi::xml_node &&node) : Map_site(node) {}

    virtual void draw(sf::RenderTarget &target) const override;
    void set_shape(const sf::Vector2f &pos1, const sf::Vector2f &pos2,
                   const float &radius);

    DEFINE_MAP_SITE_NAMING(River)

   private:
    Hex_border_shape _shape;
};
REGISTER_MAP_SITE(River)