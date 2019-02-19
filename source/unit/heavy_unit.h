#pragma once

#include "mover/concrete_mover.h"
#include "unit.h"

class Armoured_cavalary : public Unit {
   public:
    Armoured_cavalary() : Unit(5) {}

    inline Unit_type get_type() const override { return Unit_type::armoured_cavalary; }
    inline void set_color() override { _token.setFillColor(sf::Color(120, 0, 0)); }
    inline Mover* get_mover() override { return new Tracks_mover(static_cast<Unit*>(this)); }
};

class Mechanized : public Unit {
   public:
    Mechanized() : Unit(8) {}

    inline Unit_type get_type() const override { return Unit_type::mechanized; }
    inline void set_color() override { _token.setFillColor(sf::Color(0, 50, 140)); }
    inline Mover* get_mover() override { return new Tracks_mover(static_cast<Unit*>(this)); }
};