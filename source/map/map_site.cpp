#include "map_site.h"

#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/archives/json.hpp>

bool Map_site::is_highlighted() const noexcept {
    return _highlighted;
}

void Map_site::set_highlighted(bool highlighted) {
    _highlighted = highlighted;
}