#pragma once

#include "messaging.h"

#include <cereal/types/utility.hpp>
#include <cereal/types/vector.hpp>

struct Unit_moved_msg : public Message {
   public:
    Unit_moved_msg() = default;
    Unit_moved_msg(const int& unit_id, const int& destination_id, const int& cost);

    int _unit_id{};
    int _dest_id{};
    int _cost{};

    std::string log() const final;

    template <class Archive>
    void serialize(Archive& ar) { ar(_unit_id, _dest_id, _cost); }
};
