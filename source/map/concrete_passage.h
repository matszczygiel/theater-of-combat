#pragma once

#include "passage_site.h"
#include "shapes/hex_border_shape.h"

#include <SFML/System/Vector2.hpp>
#include <cereal/types/polymorphic.hpp>

class River : public Passage_site {
   public:
    explicit River(const int &number = 0);

    virtual void draw(sf::RenderTarget &target) const final;
    void set_shape(const sf::Vector2f &pos1, const sf::Vector2f &pos2,
                   const float &radius) final;

    Map_site::Type get_type() const final;

   private:
    Hex_border_shape _shape;

   public:
    template <class Archive>
    void serialize(Archive &ar) {
        ar(cereal::virtual_base_class<Passage_site>(this));
    }
};
inline Map_site::Type River::get_type() const { return Map_site::Type::River; }
