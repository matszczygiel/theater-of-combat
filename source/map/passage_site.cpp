#include "passage_site.h"

#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/types/polymorphic.hpp>

CEREAL_REGISTER_TYPE(Passage_site)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Map_site, Passage_site);