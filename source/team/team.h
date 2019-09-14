#ifndef TEAM_H
#define TEAM_H

#include <string>
#include <set>

#include "unit/unit.h"
#include "map/map.h"

class Team {
   public:
    explicit Team(const std::string& name, Map& map, Unit_set& set)
        : _map(&map), _unit_set(&set), _name(name) {}

    bool remove_unit_ownership(const int& no);
    bool add_unit_ownership(const int& no);
    std::set<int> get_controlable_hexes() const;
    void draw(sf::RenderTarget& target) const;
    void update();
    std::set<Unit::IdType> get_units_controling(const Map::SiteId& hex_id) const;
    std::set<Unit::IdType> get_units_controling(const std::vector<Map::SiteId>& hex_ids) const;


    template <class Archive>
    void serialize(Archive& ar);

   private:
    std::string _name{};
    std::set<Unit::IdType> _owned_units{};
};

template <class Archive>
void Team::serialize(Archive& ar) {
    ar(_owned_units_ids, _name);
}

#endif