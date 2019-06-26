#include "unit_set.h"

#include "unit.h"

Unit* Unit_set::get_by_id(const int& id) {
    return _units.at(id).get();
}

bool Unit_set::remove_by_id(const int& id) {
    return _units.erase(id) == 1;
}

void Unit_set::init_tokens(const float& size) {
    for (auto& u : _units)
        u.second->init_token(size);
}