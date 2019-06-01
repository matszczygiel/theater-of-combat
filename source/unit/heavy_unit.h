#pragma once

#include "mover/concrete_mover.h"
#include "unit.h"

class Armoured_cavalry : public Unit {
   public:
    explicit Armoured_cavalry(const std::string& description) noexcept
        : Unit(10, 10, description) {}
    explicit Armoured_cavalry(pugi::xml_node& node) : Unit(node) {}

    std::unique_ptr<Mover> get_mover(std::shared_ptr<Map>& map) final;
    int texture_offset() const final;

    DEFINE_UNIT_NAMING(Armoured_cavalry)
};
REGISTER_UNIT(Armoured_cavalry);

class Mechanized : public Unit {
   public:
    explicit Mechanized(const std::string& description) noexcept
        : Unit(15, 10, description) {}
    explicit Mechanized(pugi::xml_node& node) : Unit(node) {}

    int texture_offset() const final;
    std::unique_ptr<Mover> get_mover(std::shared_ptr<Map>& map) final;
    DEFINE_UNIT_NAMING(Mechanized)
};
REGISTER_UNIT(Mechanized)

inline int Armoured_cavalry::texture_offset() const { return 0; }

inline std::unique_ptr<Mover> Armoured_cavalry::get_mover(std::shared_ptr<Map>& map) {
    return std::make_unique<Tracks_mover>(this, map);
}

inline int Mechanized::texture_offset() const { return 1; }

inline std::unique_ptr<Mover> Mechanized::get_mover(std::shared_ptr<Map>& map) {
    return new Tracks_mover(static_cast<Unit*>(this));
}