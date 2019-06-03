#include "concrete_message.h"

#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/xml.hpp>

#include <cereal/types/polymorphic.hpp>

Unit_move_request::Unit_move_request(const int& unit_id,
                                     const std::vector<std::pair<int, int>>& hexes)
    : _unit_id(unit_id), _hexes(hexes) {}

std::string Unit_move_request::log() const {
    std::string res = "Requested unit no " + std::to_string(_unit_id) + " through fields:";
    for (const auto& x : _hexes)
        res = +" " + std::to_string(x.first);
    return res;
}

CEREAL_REGISTER_TYPE(Unit_move_request)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Message, Unit_move_request)