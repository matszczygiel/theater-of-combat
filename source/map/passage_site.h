#pragma once

#include "map_site.h"

class Passage_site : public Map_site {
   public:
    explicit Passage_site(const int &number = 0) : Map_site(number) {}
    explicit Passage_site(pugi::xml_node &&node) : Map_site(node) {}

    virtual void set_shape(const sf::Vector2f &pos1, const sf::Vector2f &pos2,
                           const float &radius) = 0;
};