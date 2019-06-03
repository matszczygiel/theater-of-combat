#pragma once

#include "hex_site.h"

#include <cereal/types/base_class.hpp>

class Field : public Hex_site {
   public:
    explicit Field(const int& number = 0);
    Map_site::Type get_type() const final;

   private:
    sf::Color color() override;
};
inline Map_site::Type Field::get_type() const { return Map_site::Type::Field; }

class Forest : public Hex_site {
   public:
    explicit Forest(const int& number = 0);
    Map_site::Type get_type() const final;

   private:
    sf::Color color() override;
};
inline Map_site::Type Forest::get_type() const { return Map_site::Type::Forest; }
