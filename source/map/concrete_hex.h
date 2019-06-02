#pragma once

#include "hex_site.h"

#include <cereal/types/base_class.hpp>

class Field : public Hex_site {
   public:
    explicit Field(const int& number = 0) : Hex_site(number) {}
    explicit Field(pugi::xml_node&& node) : Hex_site(node) {}

    DEFINE_MAP_SITE_NAMING(Field)

   private:
    sf::Color color() override;
};
REGISTER_MAP_SITE(Field)

class Forest : public Hex_site {
   public:
    explicit Forest(const int& number = 0) : Hex_site(number) {}
    explicit Forest(pugi::xml_node&& node) : Hex_site(node) {}

    DEFINE_MAP_SITE_NAMING(Forest)

   private:
    sf::Color color() override;
};
REGISTER_MAP_SITE(Forest)

inline sf::Color Field::color() { return sf::Color::Green; }

inline sf::Color Forest::color() { return sf::Color(100, 140, 20); }
