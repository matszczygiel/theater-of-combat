#include "map_site.h"

Map_site::Map_site(pugi::xml_node& node)
    : _number(node.attribute(number_identifier).as_int()) {
}

std::unique_ptr<Map_site> Map_site::unserialize(pugi::xml_node& node) {
    const id_type type = node.attribute("type").value();
    return registrable_base::create(type, node);
}

void Map_site::serialize(pugi::xml_node& node) const {
    auto child = node.append_child(object_identifier);
    child.append_attribute(type_identifier).set_value(get_name().c_str());
    child.append_attribute(number_identifier).set_value(_number);
}

bool Map_site::is_highlighted() const noexcept {
    return _highlighted;
}

constexpr auto& Map_site::get_number() const noexcept {
    return _number;
}

void Map_site::set_highlighted(bool highlighted) {
    _highlighted = highlighted;
}