#pragma once

#include "messaging.h"

#include <cereal/types/utility.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/set.hpp>

struct Unit_moved_msg : public Message {
   public:
    Unit_moved_msg() = default;
    Unit_moved_msg(const int& unit_id_, const int& destination_id_, const int& cost_);

    int unit_id{};
    int dest_id{};
    int cost{};

    std::string log() const final;

    template <class Archive>
    void serialize(Archive& ar) { ar(unit_id, dest_id, cost); }
};

struct Battle_ended_msg : public Message {
   public:
    std::map<int, int> loses{};
    std::string loser{};
    int retreat_distance{};
    std::set<int> retreating_units{};

    std::string log() const final;

    template <class Archive>
    void serialize(Archive& ar) { ar(loses, loser); }
};
