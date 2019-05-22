#pragma once

#include "messaging.h"

class Unit_moved_msg : public Message {
   public:
    Unit_moved_msg(const std::string& stream);
    explicit Unit_moved_msg(const int& hexno, const std::string& unit_name, const std::string& nation);

    int _hexno;
    std::string _unit_name;
    std::string _nation;

    DEFINE_MESSAGE_NAMING(Unit_moved_msg);
};
REGISTER_CLASS(Message, Unit_moved_msg);