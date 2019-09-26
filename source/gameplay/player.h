#ifndef PLAYER_H
#define PLAYER_H

#include <memory>
#include <stack>
#include <string>
#include <vector>

#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>

class Player {
   public:
    Player() = default;
    Player(std::string name, const std::vector<std::string>& teams);

    const std::string& name() const;
    const std::vector<std::string> teams() const;

    template <class Archive>
    void serialize(Archive& archive);

   private:
    std::string _name{};
    std::vector<std::string> _owned_teams{};
};

template <class Archive>
void Player::serialize(Archive& archive) {
    archive(CEREAL_NVP(_name), CEREAL_NVP(_owned_teams));
}

#endif