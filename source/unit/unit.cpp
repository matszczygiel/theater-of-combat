#include "unit.h"

const std::string& Unit::name() const { return _name; }

UnitType Unit::type() { return _type; }

Unit::IdType ComponentBase::owner() { return _owner; }

UnitType ComponentBase::owner_type() { return _owner_type; }


