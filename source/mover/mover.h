#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "map/map_site.h"
#include "unit/unit.h"
#include "map/passage_site.h"

class Mover {
   public:
    explicit Mover(Unit* unit)
        : _unit(unit), _paths(), _passages(), _hex_table(), _pass_table() {}

    void find_paths();
    void move(const sf::Vector2f& mouse_pos);

    virtual ~Mover() = default;

   protected:
    virtual void create_table() = 0;

    void clear();

    Unit* _unit;
    std::vector<std::unordered_set<Hex_site*>> _paths;
    std::unordered_set<Passage_site*> _passages;
    std::unordered_map<Hex_type, int> _hex_table;
    std::unordered_map<Passage_type, int> _pass_table;
};