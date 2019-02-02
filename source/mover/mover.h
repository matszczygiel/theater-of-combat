#pragma once

#include <unordered_set>
#include <vector>

#include "map_site.h"
#include "unit.h"

class Mover {
   public:
    virtual void move(const sf::Vector2f& mouse_pos);
    inline void set_unit(Unit* unit) {
        _unit = unit;
        find_paths();
    }

   private:
    void find_paths();
    void clear();

    Unit* _unit;
    std::vector<std::unordered_set<Map_site*>> _paths;
};