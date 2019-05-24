#include "concrete_hex.h"

#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/xml.hpp>

#include <cereal/types/polymorphic.hpp>

CEREAL_REGISTER_TYPE(Field)
CEREAL_REGISTER_TYPE(Forest)
