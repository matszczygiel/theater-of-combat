#pragma once

#include "mover/concrete_mover.h"
#include "unit.h"

class Armoured_cavalary : public Unit {
   public:
    Armoured_cavalary() : Unit(10, 10) {}

    Unit_type get_type() const final;
    Mover* get_mover(std::weak_ptr<Message_bus> bus) final;
    int texture_offset() const final;
};

class Mechanized : public Unit {
   public:
    Mechanized() : Unit(15, 10) {}

    Unit_type get_type() const final;
    int texture_offset() const final;
    Mover* get_mover(std::weak_ptr<Message_bus> bus) final;
};

inline Unit_type Armoured_cavalary::get_type() const { return Unit_type::armoured_cavalary; }

inline int Armoured_cavalary::texture_offset() const { return 0; }

inline Mover* Armoured_cavalary::get_mover(std::weak_ptr<Message_bus> bus) { return new Tracks_mover(bus, static_cast<Unit*>(this)); }

inline Unit_type Mechanized::get_type() const { return Unit_type::mechanized; }

inline int Mechanized::texture_offset() const { return 1; }

inline Mover* Mechanized::get_mover(std::weak_ptr<Message_bus> bus) { return new Tracks_mover(bus, static_cast<Unit*>(this)); }