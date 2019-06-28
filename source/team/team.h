#pragma once

#include <map>
#include <memory>
#include <set>
#include <string>

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
    std::set<int> get_controlable_hexes() const;
    void draw(sf::RenderTarget& target) const;
    void update();
    void apply(const std::function<void(Unit&)>& function);
    std::set<Unit*> get_units_controling(const int& hex_id);
    std::set<Unit*> get_units_controling(const std::vector<int>& hex_ids);

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
