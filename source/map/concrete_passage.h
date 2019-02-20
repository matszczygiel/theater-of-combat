#pragma once

#include "SFML/Graphics/RectangleShape.hpp"

#include "passage_site.h"
#include "hex_border_shape.h"

class River : public Passage_site {
   public:
    explicit River(const int &number = 0) 
    : Passage_site(number), _shape(0, 0.2) {}

    Site_type get_type() const final;
    void draw(sf::RenderTarget &target) const final;

   private:
    void set_shape() final;

    Hex_border_shape _shape;
};

inline Site_type River::get_type() const { return Site_type::river; }