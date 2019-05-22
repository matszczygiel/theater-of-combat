#pragma once

#include "messaging.h"

class Unit_moved_msg : public Message {
   public:
    Unit_moved_msg(const std::string& stream);
    explicit Unit_moved_msg(const int& hexno, const std::string& unit_name, const std::string& nation);

    const int _hexno;
    const std::string _unit_name;
    const std::string _nation;

    static const Message::id_type name;
    virtual const Message::id_type& get_name() const override;
};
REGISTER_CLASS(Message, Unit_moved_msg);