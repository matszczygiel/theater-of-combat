#include "map_site.h"

Map_site::Map_site(pugi::xml_node& node)
    : _number(node.attribute(number_identifier).as_int()) {
}

Map_site::ptr_base Map_site::unserialize(pugi::xml_node& node) {
    const id_type type = node.attribute("type").value();
    return registrable_base::create(type, node);
}

void Map_site::serialize(pugi::xml_node& node) const {
    auto child = node.append_child(object_identifier);
    child.append_attribute(type_identifier).set_value(get_name().c_str());
    child.append_attribute(number_identifier).set_value(_number);
}
