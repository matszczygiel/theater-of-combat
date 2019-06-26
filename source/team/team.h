#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include <SFML/Graphics/RenderTarget.hpp>
#include <cereal/types/set.hpp>

class Unit;
class Unit_set;
class Map;

class Team {
   public:
    explicit Team(const std::string& name, Map& map, Unit_set& set)
        : _map(&map), _unit_set(&set), _name(name) {}

    Team() = default;

    bool remove_unit_ownership(const int& no);
    bool add_unit_ownership(const int& no);
    std::vector<int> get_controlable_hexes() const;
    void draw(sf::RenderTarget& target) const;
    void update();

    template <class Archive>
    void serialize(Archive& ar);

   private:
    std::set<int> _owned_units_ids{};
    std::vector<Unit*> _units_to_draw{};
    Map* _map{nullptr};
    Unit_set* _unit_set{nullptr};
    std::string _name{};
};

template <class Archive>
void Team::serialize(Archive& ar) {
    ar(_owned_units_ids, _name);
}
