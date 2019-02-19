#pragma once

#include "SFML/Graphics/RectangleShape.hpp"

#include "passage_site.h"

class River : public Passage_site {
   public:
    explicit River(const int &number = 0) : Passage_site(number) {}

    Site_type get_type() const override;

   protected:
    void set_shape() override;
};

inline Site_type River::get_type() const { return Site_type::river; }