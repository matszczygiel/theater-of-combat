#pragma once

#include "messaging.h"

#include <cereal/types/utility.hpp>
#include <cereal/types/vector.hpp>

struct Unit_move_request : public Message {
   public:
    Unit_move_request() = default;
    Unit_move_request(const int& unit_id,
                      const std::vector<std::pair<int, int>>& hexes);

    int _unit_id;
    std::vector<std::pair<int, int>> _hexes;

    virtual std::string log() const override;
    virtual ~Unit_move_request() = default;

    template <class Archive>
    void serialize(Archive& ar) { ar(_unit_id, _hexes); }
};
