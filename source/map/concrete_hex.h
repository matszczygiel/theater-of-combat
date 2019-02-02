#pragma once

#include "hex_site.h"

class Field : public Hex_site {
   public:
    explicit Field(const int& number) : Hex_site(number) {}

    Site_type get_type() const override { return Site_type::field; }

   private:
    void set_color() override { _shape.setFillColor(sf::Color::Green); }
};

class Forest : public Hex_site {
   public:
    explicit Forest(const int& number) : Hex_site(number) {}

    Site_type get_type() const override { return Site_type::forest; }

   private:
    void set_color() override { _shape.setFillColor(sf::Color(100, 140, 20)); }
};
