#pragma once

#include "hex_site.h"

class Field : public Hex_site {
   public:
    explicit Field(const int& number = 0) : Hex_site(number) {}

    Hex_type get_hex_type() const final;

   private:
    void set_color() override;

   public:
    template <class Archive>
    void serialize(Archive& ar) { ar(cereal::virtual_base_class<Hex_site>(this)); }
};

class Forest : public Hex_site {
   public:
    explicit Forest(const int& number = 0) : Hex_site(number) {}

    Hex_type get_hex_type() const final;

   private:
    void set_color() override;

   public:
    template <class Archive>
    void serialize(Archive& ar) { ar(cereal::virtual_base_class<Hex_site>(this)); }
};

inline Hex_type Field::get_hex_type() const { return Hex_type::field; }

inline void Field::set_color() { _shape.setFillColor(sf::Color::Green); }

inline Hex_type Forest::get_hex_type() const { return Hex_type::forest; }

inline void Forest::set_color() { _shape.setFillColor(sf::Color(100, 140, 20)); }
