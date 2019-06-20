#pragma once

#include "map_site.h"

#include <cereal/types/polymorphic.hpp>

class Passage_site : public Map_site {
   public:
    enum class Type {
        River
    };

    explicit Passage_site(const int &number) noexcept
        : Map_site(number) {}

    Passage_site()          = default;
    virtual ~Passage_site() = default;

    virtual void set_shape(const sf::Vector2f &pos1, const sf::Vector2f &pos2,
                           const float &radius) = 0;

    virtual Type get_type() const = 0;
};
