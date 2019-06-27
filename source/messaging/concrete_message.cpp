#include "concrete_message.h"

#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/xml.hpp>

#include <cereal/types/polymorphic.hpp>

Unit_moved_msg::Unit_moved_msg(const int& unit_id, const int& destination_id, const int& cost)
    : _unit_id(unit_id), _dest_id(destination_id), _cost(cost) {}

std::string Unit_moved_msg::log() const {
    std::string res = "Unit no " + std::to_string(_unit_id) + " moved to hex " + std::to_string(_dest_id) +
                      " paying " + std::to_string(_cost) + " moving points.";

    return res;
}

CEREAL_REGISTER_TYPE(Unit_moved_msg)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Message, Unit_moved_msg)