#include "concrete_hex.h"

#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/xml.hpp>

#include <cereal/types/polymorphic.hpp>

Field::Field(const int& number) : Hex_site(number) {}

sf::Color Field::color() { return sf::Color::Green; }

CEREAL_REGISTER_TYPE(Field);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Hex_site, Field);

Forest::Forest(const int& number) : Hex_site(number) {}

sf::Color Forest::color() { return sf::Color(100, 140, 20); }

CEREAL_REGISTER_TYPE(Forest);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Hex_site, Forest);
