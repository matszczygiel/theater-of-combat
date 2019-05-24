#pragma once

#include <cereal/types/vector.hpp>

struct Message {
    virtual ~Message() = default;
    template <class Archive>
    void serialize(Archive& ar) {}
};

struct Unit_move_request : public Message {
    int _unit_id;
    std::vector<int> _hex_ids;

    template <class Archive>
    void serialize(Archive& ar) { ar(_unit_id, _hex_ids); }
};