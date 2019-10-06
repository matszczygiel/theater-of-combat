#include "unit.h"

const std::string& Unit::name() const noexcept { return _name; }

UnitType Unit::type() const noexcept { return _type; }

Unit::IdType ComponentBase::owner() const noexcept { return _owner; }

UnitType ComponentBase::owner_type() const noexcept { return _owner_type; }

