#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "map/map_site.h"
#include "map/site_type.h"
#include "unit/unit.h"
#include "map/passage_site.h"

class Mover {
   public:
    explicit Mover(Unit* unit)
        : _unit(unit), _paths(), _passages(), _table() {}

    void find_paths();
    void move(const sf::Vector2f& mouse_pos);

    virtual ~Mover() = default;

   protected:
    virtual void create_table() = 0;

    void clear();

    Unit* _unit;
    std::vector<std::unordered_set<Hex_site*>> _paths;
    std::unordered_set<Passage_site*> _passages;
    std::unordered_map<Site_type, int> _table;
};