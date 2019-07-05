#include "concrete_message.h"

#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/xml.hpp>

#include <cereal/types/polymorphic.hpp>

Unit_moved_msg::Unit_moved_msg(const int& unit_id_, const int& destination_id_, const int& cost_)
    : unit_id(unit_id_), dest_id(destination_id_), cost(cost_) {}

std::string Unit_moved_msg::log() const {
    std::string res = "Unit no " + std::to_string(unit_id) + " moved to hex " + std::to_string(dest_id) +
                      " paying " + std::to_string(cost) + " moving points.";

    return res;
}

CEREAL_REGISTER_TYPE(Unit_moved_msg)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Message, Unit_moved_msg)

Battle_ended_msg::Battle_ended_msg(const std::map<int, int>& loses_) : loses(loses_) {}

std::string Battle_ended_msg::log() const {
    std::string res = "Loses:";
    bool comma      = false;
    for (const auto& x : loses) {
        if (comma)
            res += ",";
        else
            comma = true;
        res += " unit " + std::to_string(x.first) + " - " + std::to_string(x.second);
    }
    return res + "\n";
}

CEREAL_REGISTER_TYPE(Battle_ended_msg)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Message, Battle_ended_msg)