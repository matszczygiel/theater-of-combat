#pragma once

#include "messaging.h"

#include <cereal/types/vector.hpp>

struct Unit_move_request : public Message {
   public:
    int _unit_id;
    std::vector<int> _hex_ids;

    virtual std::string log() const override;
    virtual ~Unit_move_request() = default;

    template <class Archive>
    void serialize(Archive& ar) { ar(_unit_id, _hex_ids); }
};
