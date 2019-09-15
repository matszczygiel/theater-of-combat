#include "team.h"

bool Team::remove_unit_ownership(const int& no) {
    return _owned_units.erase(no) == 1;
}

bool Team::add_unit_ownership(const int& no) {
    return _owned_units.insert(no).second;
}

const std::string& Team::name() const { return _name; }

const std::set<Unit::IdType> Team::units() const { return _owned_units; }
