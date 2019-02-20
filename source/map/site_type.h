#pragma once

enum class Site_type {
    field     = 1,
    forest    = 2,
    hills     = 3,
    mountains = 4,
    swamp     = 5,

    stream     = -1,
    creek      = -2,
    river      = -3,
    huge_river = -4,
    road       = -5,
};

inline bool is_site_type_placeable(const Site_type& type) {
    return static_cast<int>(type) > 0 ? true : false;
}
