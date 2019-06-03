#include "heavy_unit.h"

#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/types/polymorphic.hpp>

#include "mover/concrete_mover.h"

int Armoured_cavalry::texture_offset() const { return 0; }

std::unique_ptr<Mover> Armoured_cavalry::get_mover(std::shared_ptr<Map>& map) {
    return std::make_unique<Tracks_mover>(shared_from_this(), map);
}

CEREAL_REGISTER_TYPE(Armoured_cavalry);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Unit, Armoured_cavalry);

int Mechanized::texture_offset() const { return 1; }

std::unique_ptr<Mover> Mechanized::get_mover(std::shared_ptr<Map>& map) {
    return std::make_unique<Tracks_mover>(shared_from_this(), map);
}

CEREAL_REGISTER_TYPE(Mechanized);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Unit, Mechanized);
