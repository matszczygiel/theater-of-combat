#pragma once

#include "hex_site.h"

class Field : public Hex_site {
   public:
    explicit Field(const int& number) : Hex_site(number) {}

    Site_type get_type() const override;

   private:
    void set_color() override;
};

class Forest : public Hex_site {
   public:
    explicit Forest(const int& number) : Hex_site(number) {}

    Site_type get_type() const override;

   private:
    void set_color() override;
};

inline Site_type Field::get_type() const { return Site_type::field; }

inline void Field::set_color() { _shape.setFillColor(sf::Color::Green); }

inline Site_type Forest::get_type() const { return Site_type::forest; }

inline void Forest::set_color() { _shape.setFillColor(sf::Color(100, 140, 20)); }
