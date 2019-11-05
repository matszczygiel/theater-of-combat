#include "unit/unit.h"

const std::string& Unit::name() const noexcept { return _name; }

Unit::KindType Unit::type() const noexcept { return _type; }
