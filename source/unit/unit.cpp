#include "unit.h"

const std::string& Unit::name() const { return _name; }

UnitType Unit::type() { return _type; }

Unit::IdType ComponentBase::owner() const { return _owner; }

UnitType ComponentBase::owner_type() const { return _owner_type; }


