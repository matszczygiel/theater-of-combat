#ifndef COMPONENTS_INFO_H
#define COMPONENTS_INFO_H

#include "toc/unit/unit_manager.h"

void show_unit_info(const Unit& unit, const Unit::IdType& id, const UnitManager& um,
                    bool* open);

#endif