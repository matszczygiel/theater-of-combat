#include "unit_set.h"

Unit* Unit_set::get_by_id(const int& id) {
    return _units.at(id).get();
}

bool Unit_set::remove_by_id(const int& id) {
    return _units.erase(id) == 1;
}