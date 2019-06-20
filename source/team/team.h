#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include <SFML/Graphics/RenderTarget.hpp>

#include "map/map.h"

class Unit;

class Team {
   public:
    explicit Team(const std::string& name = "") : _name(name) {}

    void add_unit(std::unique_ptr<Unit> unit);
    void remove_unit(const int& no);
    std::vector<int> get_controlable_hexes(const Map& map) const;
    void draw(sf::RenderTarget& target) const;

   private:
    std::map<int, std::unique_ptr<Unit>> _units;
    std::vector<Unit*> _units_to_draw;
    const std::string _name;
};