#pragma once

#include "log.h"

enum class Directions {
    northeast = 0,
    east      = 1,
    southeast = 2,
    southwest = 3,
    west      = 4,
    northwest = 5
};

inline std::string direction_to_string(const Directions &dir) {
    switch (dir) {
        case Directions::northeast:
            return "northeast";
        case Directions::east:
            return "east";
        case Directions::southeast:
            return "southeast";
        case Directions::southwest:
            return "southwest";
        case Directions::west:
            return "west";
        case Directions::northwest:
            return "northwest";
        default:
            GAME_WARN("Invalid direction type to string conversion.");
            return "";
    }
}

inline Directions string_to_direction(const std::string &str) {
    if (str == "northeast")
        return Directions::northeast;
    else if (str == "east")
        return Directions::east;
    else if (str == "southeast")
        return Directions::southeast;
    else if (str == "southwest")
        return Directions::southwest;
    else if (str == "west")
        return Directions::west;
    else if (str == "northwest")
        return Directions::northwest;
    else {
        GAME_ERROR("Invalid string to direction type conversion.");
        throw std::runtime_error("Invalid string to direction type conversion.");
    }
}