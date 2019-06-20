#pragma once

#include <set>
#include <string>

#include "unit/unit.h"

class Player {
   public:
   explicit Player(const std::string& name = ""): _name(name) {}
    std::set<std::shared_ptr<Unit> >& get_players_units();
    void set_name(const std::string& name);
    void add_unit(std::shared_ptr<Unit>  unit);

   private:
    std::string _name;
    std::set<std::shared_ptr<Unit> > _units;
};

inline std::set<std::shared_ptr<Unit> >& Player::get_players_units() { return _units; }
inline void Player::set_name(const std::string& name) { _name = name; }