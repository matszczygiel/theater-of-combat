#pragma once

#include "hex_site.h"

#include <cereal/types/base_class.hpp>

class Field : public Hex_site {
   public:
    explicit Field(const int& number = 0);

   private:
    sf::Color color() override;
};

class Forest : public Hex_site {
   public:
    explicit Forest(const int& number = 0);

   private:
    sf::Color color() override;
};
