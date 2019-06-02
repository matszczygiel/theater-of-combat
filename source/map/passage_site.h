#pragma once

#include "map_site.h"

#include <cereal/types/polymorphic.hpp>

class Passage_site : public Map_site {
   public:
    explicit Passage_site(const int &number = 0) noexcept
        : Map_site(number) {}

    virtual void set_shape(const sf::Vector2f &pos1, const sf::Vector2f &pos2,
                           const float &radius) = 0;

};

