#include "concrete_message.h"

#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/xml.hpp>

#include <cereal/types/polymorphic.hpp>

std::string Unit_move_request::log() const {
    std::string res = "Requested unit no " + std::to_string(_unit_id) + " through fields:";
    for(const auto& x : _hex_ids)
        res =+ " " + std::to_string(x);
    return res; 
}

CEREAL_REGISTER_TYPE(Unit_move_request)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Message, Unit_move_request)