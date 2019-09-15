#ifndef TEAM_H
#define TEAM_H

#include <string>
#include <set>

#include <cereal/types/set.hpp>

#include "unit.h"

class Team {
   public:
    Team() = default;
    explicit Team(const std::string& name);

    bool remove_unit_ownership(const int& no);
    bool add_unit_ownership(const int& no);

    const std::string& name() const;
    const std::set<Unit::IdType> units() const;

    template <class Archive>
    void serialize(Archive& ar);

   private:
    std::string _name{};
    std::set<Unit::IdType> _owned_units{};
};

template <class Archive>
void Team::serialize(Archive& ar) {
    ar(_name, _owned_units);
}

#endif