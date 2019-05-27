#pragma once

#include "passage_site.h"
#include "shapes/hex_border_shape.h"

#include <SFML/System/Vector2.hpp>

class River : public Passage_site {
   public:
    explicit River(const int &number = 0) : Passage_site(number) {}
    explicit River(pugi::xml_node &&node) : Passage_site(node) {}

    virtual void draw(sf::RenderTarget &target) const final;
    void set_shape(const sf::Vector2f &pos1, const sf::Vector2f &pos2,
                   const float &radius) final;

    DEFINE_MAP_SITE_NAMING(River)

   private:
    Hex_border_shape _shape;
};
REGISTER_MAP_SITE(River)