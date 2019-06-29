#pragma once

#include <map>
#include <set>

#include "unit/unit.h"

class Battlefield {
   public:
    void push(Unit* unit, const std::string& owner_name);
    void push(std::set<Unit*> units, const std::string& owner_name);

    void carry_fight();

   private:
    std::map<std::string, std::set<Unit*>> _bucket;
};
