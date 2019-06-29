#pragma once

#include <map>
#include <string>

#include "team/team.h"

class Player {
   public:
    explicit Player(const std::string& name = "") : _name(name) {}
    void set_name(const std::string& name);
    const std::string& name() const;

    template <class Archive>
    void serialize(Archive& ar);

    std::vector<Team> teams;

   private:
    std::string _name;
};

template <class Archive>
void Player::serialize(Archive& ar) {
    ar(_name, teams);
}
