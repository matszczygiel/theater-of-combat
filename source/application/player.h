#pragma once

#include <string>
#include <vector>
#include <set>

#include "unit/unit.h"
#include "unit/stack.h"

class Player {
   public:

   
   private:
    std::string _name;

    std::vector<std::unique_ptr<Unit>> _units;
    std::set<Unit*> _units_to_draw;
    std::vector<Stack> _stacks;
};