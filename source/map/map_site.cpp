#include "map_site.h"

std::string type_to_string(const Site_type& type) {
    switch (type) {
        case Site_type::hexagon:
            return "hexagon";
        case Site_type::passage:
            return "passage";
        default:
            GAME_WARN("Invalid site type to string conversion.");
            return "";
    }
}

Site_type string_to_site_type(const std::string& str) {
    if (str == "hexagon")
        return Site_type::hexagon;
    else if (str == "passage")
        return Site_type::passage;
    else {
        GAME_ERROR("Invalid string to site type conversion.");
        throw std::runtime_error("Invalid string to site type conversion.");
    }
}

void Map_site::write(pugi::xml_node& node) {
    auto site_node = node.append_child("site");
    site_node.append_attribute("number").set_value(_number);
    site_node.append_attribute("site_type").set_value(type_to_string(get_site_type()).c_str());
}
