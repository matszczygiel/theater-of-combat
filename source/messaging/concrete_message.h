#pragma once

#include "messaging.h"

class Request_unit_movement : public Message {
   public:
    Request_unit_movement(const std::string& stream);
    explicit Request_unit_movement(const int& hexno, const std::string& unit_name, const std::string& nation);

    std::string to_string() const override;

    int _hexno;
    std::string _unit_name;
    std::string _nation;

    DEFINE_MESSAGE_NAMING(Request_unit_movement)
};
REGISTER_CLASS(Message, Request_unit_movement)