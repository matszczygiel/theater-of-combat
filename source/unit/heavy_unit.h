#pragma once

#include "mover/concrete_mover.h"
#include "unit.h"

class Armoured_cavalary : public Unit {
   public:
    explicit Armoured_cavalary() : Unit(5) {}

    Unit_type get_type() const override { return Unit_type::armoured_cavalary; }
    void set_color() override { _token.setFillColor(sf::Color(120, 0, 0)); }
    Mover* get_mover() override { return new Tracks_mover(static_cast<Unit*>(this)); }
};

class Mechanized : public Unit {
   public:
    explicit Mechanized() : Unit(8) {}

    Unit_type get_type() const override { return Unit_type::mechanized; }
    void set_color() override { _token.setFillColor(sf::Color(0, 50, 140)); }
    Mover* get_mover() override { return new Tracks_mover(static_cast<Unit*>(this)); }
};