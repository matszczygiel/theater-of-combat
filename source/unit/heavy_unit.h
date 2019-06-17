#pragma once

#include "mover/concrete_mover.h"
#include "unit.h"

class Armoured_cavalry : public Unit {
   public:
    explicit Armoured_cavalry(const std::string& description = "") noexcept
        : Unit(10, 10, description) {}

    std::unique_ptr<Mover> get_mover(std::shared_ptr<Map>& map) final;
    int texture_offset() const final;

};

class Mechanized : public Unit {
   public:
    explicit Mechanized(const std::string& description = "") noexcept
        : Unit(15, 10, description) {}

    int texture_offset() const final;
    std::unique_ptr<Mover> get_mover(std::shared_ptr<Map>& map) final;
};

