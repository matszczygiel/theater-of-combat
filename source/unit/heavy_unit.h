#pragma once

#include "mover/concrete_mover.h"
#include "unit.h"

class Armoured_cavalary : public Unit {
   public:
    Armoured_cavalary() : Unit(10) {}

    Unit_type get_type() const final;
    void set_color() final;
    Mover* get_mover() final;
};

class Mechanized : public Unit {
   public:
    Mechanized() : Unit(15) {}

    Unit_type get_type() const final;
    void set_color() final;
    Mover* get_mover() final;
};

inline Unit_type Armoured_cavalary::get_type() const { return Unit_type::armoured_cavalary; }

inline void Armoured_cavalary::set_color() { _token.setFillColor(sf::Color(120, 0, 0)); }

inline Mover* Armoured_cavalary::get_mover() { return new Tracks_mover(static_cast<Unit*>(this)); }

inline Unit_type Mechanized::get_type() const { return Unit_type::mechanized; }

inline void Mechanized::set_color() { _token.setFillColor(sf::Color(0, 50, 140)); }

inline Mover* Mechanized::get_mover() { return new Tracks_mover(static_cast<Unit*>(this)); }