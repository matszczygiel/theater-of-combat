#include "unit.h"

const std::string& Unit::name() const noexcept { return _name; }

UnitType Unit::type() const noexcept { return _type; }
